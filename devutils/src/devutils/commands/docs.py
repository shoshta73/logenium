# SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

import os
import shutil
import subprocess
import sys
import threading
from concurrent.futures import ThreadPoolExecutor, as_completed
from pathlib import Path

import typer

from devutils.constants.paths.files import DOXYGEN_CONFIGS
from devutils.utils.filesystem import find_files_by_extensions

docs: typer.Typer = typer.Typer()


def check_doxygen_is_available() -> bool:
    if shutil.which("doxygen") is None:
        typer.echo(typer.style("Error: Doxygen is not available. Please ensure it is installed.", fg="red", bold=True))
        return False
    return True


_thread_counter = 0
_thread_id_map: dict[object, int] = {}
_thread_id_lock = threading.Lock()


def _get_thread_id() -> int:
    global _thread_counter, _thread_id_map
    thread_ident = threading.current_thread().ident
    with _thread_id_lock:
        if thread_ident not in _thread_id_map:
            _thread_counter += 1
            _thread_id_map[thread_ident] = _thread_counter
        return _thread_id_map[thread_ident]


def build_single_project(
    project_name: str,
    config_path: Path,
    ci: bool,
    _unused_thread_id: int,
    output_lock: threading.Lock,
) -> bool:
    thread_id = _get_thread_id()

    def print_with_prefix(msg: str, color: str | None = None, bold: bool = False) -> None:
        prefix = typer.style(f"[T{thread_id}]", fg="magenta", bold=True)
        if color:
            styled_msg = typer.style(msg, fg=color, bold=bold)
            typer.echo(f"{prefix} {styled_msg}")
        else:
            typer.echo(f"{prefix} {msg}")

    with output_lock:
        print_with_prefix(f"Building docs for {project_name}...", "cyan", bold=True)

    cwd = config_path.parent

    try:
        process = subprocess.Popen(
            ["doxygen", str(config_path)],
            cwd=cwd,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True,
            bufsize=1,
        )

        if process.stdout:
            for line in process.stdout:
                line = line.rstrip()
                if line:
                    with output_lock:
                        print_with_prefix(line)

        returncode = process.wait()

        if returncode != 0:
            with output_lock:
                print_with_prefix(f"Error: Failed to build docs for {project_name}.", "red", bold=True)
            return False

        if ci:
            docs_dir = config_path.parent / "docs"
            temp_files = find_files_by_extensions(docs_dir, [".map", ".md5"])
            for file in temp_files:
                with output_lock:
                    print_with_prefix(f"Removing {file}...", "cyan", bold=True)
                file.unlink()

        with output_lock:
            print_with_prefix(f"Successfully built docs for {project_name}", "green", bold=True)

        return True

    except Exception as e:
        with output_lock:
            print_with_prefix(f"Error building {project_name}: {e}", "red", bold=True)
        return False


@docs.command()  # type: ignore[misc]
def build(ci: bool = typer.Option(False, "--ci", help="Ci mode, removes unnecessary files")) -> None:
    global _thread_counter, _thread_id_map

    if not check_doxygen_is_available():
        sys.exit(1)

    for _project_name, config_path in DOXYGEN_CONFIGS.items():
        if not config_path.exists():
            typer.echo(typer.style(f"Error: Doxygen config file {config_path} does not exist.", fg="red", bold=True))
            sys.exit(1)

    # Reset thread ID tracking for each build invocation
    _thread_counter = 0
    _thread_id_map = {}

    output_lock = threading.Lock()
    max_workers = min(os.cpu_count() or 1, len(DOXYGEN_CONFIGS))
    success = True

    with ThreadPoolExecutor(max_workers=max_workers) as executor:
        future_to_project = {}
        for project_name, config_path in DOXYGEN_CONFIGS.items():
            future = executor.submit(
                build_single_project,
                project_name,
                config_path,
                ci,
                0,
                output_lock,
            )
            future_to_project[future] = project_name

        for future in as_completed(future_to_project):
            project_name = future_to_project[future]
            try:
                if not future.result():
                    success = False
            except Exception as e:
                with output_lock:
                    typer.echo(typer.style(f"[ERROR] Unexpected error for {project_name}: {e}", fg="red", bold=True))
                success = False

    if success:
        typer.echo(typer.style("\nDocumentation built successfully!", fg="green", bold=True))
    else:
        sys.exit(1)


@docs.callback(invoke_without_command=True)  # type: ignore[misc]
def main(ctx: typer.Context) -> None:
    if ctx.invoked_subcommand is None:
        build()
