# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

import json
import shlex
import shutil
import subprocess
from typing import TypedDict, cast

import jsonschema
import typer
import yaml

from devutils.constants.paths import CodegenFiles, ConfigFiles, Directories, JsonSchemas

configure: typer.Typer = typer.Typer()


class LibraryConfig(TypedDict):
    enable_testing: bool


class CorelibLibraryConfig(LibraryConfig):
    enable_tracing: bool


class LoggingLibraryConfig(LibraryConfig):
    use_fmtlib: bool
    enable_color_logs: bool


class DebugLibraryConfig(LibraryConfig):
    use_fast_stacktrace: bool
    use_fmtlib: bool
    enable_color_logs: bool


class DebugLibraryConfigR3(LibraryConfig):
    use_fast_stacktrace: bool


class ConfigurationR1(TypedDict):
    revision: int
    enable_testing: bool
    enable_xheader_testing: bool
    enable_debug_testing: bool
    enable_corelib_testing: bool
    build_mode: str


class ConfigurationR2(TypedDict):
    revision: int
    enable_testing: bool
    build_mode: str
    logenium: LibraryConfig
    xheader: LibraryConfig
    debug: DebugLibraryConfig
    corelib: CorelibLibraryConfig
    logging: LoggingLibraryConfig


class ConfigurationR3(TypedDict):
    revision: int
    enable_testing: bool
    build_mode: str
    logenium: LibraryConfig
    xheader: LibraryConfig
    debug: DebugLibraryConfigR3
    corelib: CorelibLibraryConfig
    logging: LibraryConfig


def load_schema_r1() -> object:
    with JsonSchemas.config_r1.open("r", encoding="utf-8") as f:
        schema: object = json.load(f)
    return schema


def load_schema_r2() -> object:
    with JsonSchemas.config_r2.open("r", encoding="utf-8") as f:
        schema: object = json.load(f)
    return schema


def load_configuration_r1() -> ConfigurationR1 | None:
    if not ConfigFiles.config.exists():
        return None

    with ConfigFiles.config.open("r", encoding="utf-8") as f:
        data: object = yaml.safe_load(f)

    if not isinstance(data, dict):
        typer.echo(
            typer.style("[WARNING]", fg="yellow") + " Config file is not a valid YAML dictionary",
            err=True,
        )
        return None

    if data.get("revision") != 1:
        return None

    schema = load_schema_r1()

    try:
        jsonschema.validate(instance=data, schema=schema)  # type: ignore[arg-type]
    except jsonschema.ValidationError as e:
        typer.echo(
            typer.style("[ERROR]", fg="red") + f" Config validation failed: {e.message}",
            err=True,
        )
        typer.echo(f"  Path: {'.'.join(str(p) for p in e.path)}", err=True)
        return None
    except jsonschema.SchemaError as e:
        typer.echo(
            typer.style("[ERROR]", fg="red") + f" Schema error: {e.message}",
            err=True,
        )
        return None

    return cast(ConfigurationR1, data)


def load_configuration_r2() -> ConfigurationR2 | None:
    if not ConfigFiles.config.exists():
        return None

    with ConfigFiles.config.open("r", encoding="utf-8") as f:
        data: object = yaml.safe_load(f)

    if not isinstance(data, dict):
        typer.echo(
            typer.style("[WARNING]", fg="yellow") + " Config file is not a valid YAML dictionary",
            err=True,
        )
        return None

    if data.get("revision") != 2:
        return None

    schema = load_schema_r2()

    try:
        jsonschema.validate(instance=data, schema=schema)  # type: ignore[arg-type]
    except jsonschema.ValidationError as e:
        typer.echo(
            typer.style("[ERROR]", fg="red") + f" Config validation failed: {e.message}",
            err=True,
        )
        typer.echo(f"  Path: {'.'.join(str(p) for p in e.path)}", err=True)
        return None
    except jsonschema.SchemaError as e:
        typer.echo(
            typer.style("[ERROR]", fg="red") + f" Schema error: {e.message}",
            err=True,
        )
        return None

    return cast(ConfigurationR2, data)


def save_configuration_r2(config: ConfigurationR2) -> None:
    schema = load_schema_r2()

    try:
        jsonschema.validate(instance=config, schema=schema)  # type: ignore[arg-type]
    except jsonschema.ValidationError as e:
        typer.echo(
            typer.style("[ERROR]", fg="red") + f" Config validation failed: {e.message}",
            err=True,
        )
        raise typer.Exit(1) from e

    with ConfigFiles.config.open("w", encoding="utf-8") as f:
        yaml.safe_dump(dict(config), f, default_flow_style=False, sort_keys=False)


def load_schema_r3() -> object:
    with JsonSchemas.config_r3.open("r", encoding="utf-8") as f:
        schema: object = json.load(f)
    return schema


def load_configuration_r3() -> ConfigurationR3 | None:
    if not ConfigFiles.config.exists():
        return None

    with ConfigFiles.config.open("r", encoding="utf-8") as f:
        data: object = yaml.safe_load(f)

    if not isinstance(data, dict):
        typer.echo(
            typer.style("[WARNING]", fg="yellow") + " Config file is not a valid YAML dictionary",
            err=True,
        )
        return None

    if data.get("revision") != 3:
        return None

    schema = load_schema_r3()

    try:
        jsonschema.validate(instance=data, schema=schema)  # type: ignore[arg-type]
    except jsonschema.ValidationError as e:
        typer.echo(
            typer.style("[ERROR]", fg="red") + f" Config validation failed: {e.message}",
            err=True,
        )
        typer.echo(f"  Path: {'.'.join(str(p) for p in e.path)}", err=True)
        return None
    except jsonschema.SchemaError as e:
        typer.echo(
            typer.style("[ERROR]", fg="red") + f" Schema error: {e.message}",
            err=True,
        )
        return None

    return cast(ConfigurationR3, data)


def save_configuration_r3(config: ConfigurationR3) -> None:
    schema = load_schema_r3()

    try:
        jsonschema.validate(instance=config, schema=schema)  # type: ignore[arg-type]
    except jsonschema.ValidationError as e:
        typer.echo(
            typer.style("[ERROR]", fg="red") + f" Config validation failed: {e.message}",
            err=True,
        )
        raise typer.Exit(1) from e

    with ConfigFiles.config.open("w", encoding="utf-8") as f:
        yaml.safe_dump(dict(config), f, default_flow_style=False, sort_keys=False)


def migrate_r1_to_r2(config_r1: ConfigurationR1) -> ConfigurationR2:
    config_r2: ConfigurationR2 = {
        "revision": 2,
        "enable_testing": config_r1["enable_testing"],
        "build_mode": config_r1["build_mode"],
        "logenium": {"enable_testing": config_r1["enable_testing"]},
        "xheader": {"enable_testing": config_r1["enable_xheader_testing"]},
        "debug": {
            "use_fast_stacktrace": False,
            "use_fmtlib": False,
            "enable_color_logs": False,
            "enable_testing": config_r1["enable_debug_testing"],
        },
        "corelib": {"enable_testing": config_r1["enable_corelib_testing"], "enable_tracing": False},
        "logging": {"use_fmtlib": False, "enable_color_logs": False, "enable_testing": False},
    }
    return config_r2


def migrate_r2_to_r3(config_r2: ConfigurationR2) -> ConfigurationR3:
    config_r3: ConfigurationR3 = {
        "revision": 3,
        "enable_testing": config_r2["enable_testing"],
        "build_mode": config_r2["build_mode"],
        "logenium": config_r2["logenium"],
        "xheader": config_r2["xheader"],
        "debug": {
            "use_fast_stacktrace": config_r2["debug"]["use_fast_stacktrace"],
            "enable_testing": config_r2["debug"]["enable_testing"],
        },
        "corelib": config_r2["corelib"],
        "logging": {"enable_testing": config_r2["logging"]["enable_testing"]},
    }
    return config_r3


@configure.command("migrate")  # type: ignore[misc]
def migrate_command() -> None:
    typer.echo("Migrating configuration...")

    if not ConfigFiles.config.exists():
        typer.echo(typer.style("[ERROR]", fg="red") + " No config.yaml found", err=True)
        raise typer.Exit(1)

    # Try loading r2 first (more recent)
    config_r2 = load_configuration_r2()
    if config_r2 is not None:
        typer.echo("Loaded r2 configuration:")
        typer.echo(f"  enable_testing: {config_r2['enable_testing']}")
        typer.echo(f"  logenium.enable_testing: {config_r2['logenium']['enable_testing']}")
        typer.echo(f"  xheader.enable_testing: {config_r2['xheader']['enable_testing']}")
        typer.echo(f"  debug.enable_testing: {config_r2['debug']['enable_testing']}")
        typer.echo(f"  corelib.enable_testing: {config_r2['corelib']['enable_testing']}")
        typer.echo(f"  build_mode: {config_r2['build_mode']}")

        config_r3 = migrate_r2_to_r3(config_r2)

        typer.echo("\nMigrated to r3 configuration:")
        typer.echo(f"  enable_testing: {config_r3['enable_testing']}")
        typer.echo(f"  logenium.enable_testing: {config_r3['logenium']['enable_testing']}")
        typer.echo(f"  xheader.enable_testing: {config_r3['xheader']['enable_testing']}")
        typer.echo(f"  debug.enable_testing: {config_r3['debug']['enable_testing']}")
        typer.echo(f"  corelib.enable_testing: {config_r3['corelib']['enable_testing']}")
        typer.echo(f"  build_mode: {config_r3['build_mode']}")

        if not typer.confirm("\nDo you want to save this configuration?"):
            typer.echo("Migration cancelled")
            raise typer.Exit(0)

        save_configuration_r3(config_r3)
        typer.echo(f"Configuration migrated and saved to {ConfigFiles.config.relative_to(Directories.root)}")
        return

    # Try loading r1
    config_r1 = load_configuration_r1()
    if config_r1 is None:
        typer.echo(
            typer.style("[ERROR]", fg="red") + " Config is not r1 or r2, or failed to load",
            err=True,
        )
        raise typer.Exit(1)

    typer.echo("Loaded r1 configuration:")
    typer.echo(f"  enable_testing: {config_r1['enable_testing']}")
    typer.echo(f"  enable_xheader_testing: {config_r1['enable_xheader_testing']}")
    typer.echo(f"  enable_debug_testing: {config_r1['enable_debug_testing']}")
    typer.echo(f"  enable_corelib_testing: {config_r1['enable_corelib_testing']}")
    typer.echo(f"  build_mode: {config_r1['build_mode']}")

    config_r2 = migrate_r1_to_r2(config_r1)
    config_r3 = migrate_r2_to_r3(config_r2)

    typer.echo("\nMigrated to r3 configuration:")
    typer.echo(f"  enable_testing: {config_r3['enable_testing']}")
    typer.echo(f"  logenium.enable_testing: {config_r3['logenium']['enable_testing']}")
    typer.echo(f"  xheader.enable_testing: {config_r3['xheader']['enable_testing']}")
    typer.echo(f"  debug.enable_testing: {config_r3['debug']['enable_testing']}")
    typer.echo(f"  corelib.enable_testing: {config_r3['corelib']['enable_testing']}")
    typer.echo(f"  build_mode: {config_r3['build_mode']}")

    if not typer.confirm("\nDo you want to save this configuration?"):
        typer.echo("Migration cancelled")
        raise typer.Exit(0)

    save_configuration_r3(config_r3)
    typer.echo(f"Configuration migrated and saved to {ConfigFiles.config.relative_to(Directories.root)}")


def check_codegen_files() -> None:
    required_files = [
        CodegenFiles.xdg_shell_client_header,
        CodegenFiles.xdg_shell_protocol_source,
        CodegenFiles.xdg_decoration_client_header,
        CodegenFiles.xdg_decoration_protocol_source,
    ]

    missing_files = [f for f in required_files if not f.exists()]

    if missing_files:
        typer.echo(
            f"{typer.style('[WARNING]', fg='yellow')} Required Wayland protocol files are missing:",
            err=True,
        )
        for file in missing_files:
            typer.echo(f"  - {file.relative_to(Directories.root)}", err=True)
        typer.echo(
            f"\n{typer.style('[ACTION]', fg='cyan')} Run the following command to generate them:",
            err=True,
        )
        typer.echo("  uv run devutils codegen wayland\n", err=True)
        raise typer.Exit(1)


def run(
    reconfigure: bool = typer.Option(False, "--reconfigure", help="Remove saved configuration and reconfigure"),
) -> None:
    typer.echo("Configuring the project...")
    check_codegen_files()

    if reconfigure and ConfigFiles.config.exists():
        ConfigFiles.config.unlink()
        typer.echo("Removed saved configuration")

    config_r3 = load_configuration_r3()

    if config_r3 is None and ConfigFiles.config.exists():
        # Try loading r2 first
        config_r2 = load_configuration_r2()
        if config_r2 is not None:
            typer.echo(
                typer.style("[WARNING]", fg="yellow") + " Found r2 configuration. Please migrate to r3 using:",
                err=True,
            )
            typer.echo("  uv run devutils configure migrate", err=True)
            raise typer.Exit(1)

        # Try loading r1
        config_r1 = load_configuration_r1()
        if config_r1 is not None:
            typer.echo(
                typer.style("[WARNING]", fg="yellow") + " Found r1 configuration. Please migrate to r3 using:",
                err=True,
            )
            typer.echo("  uv run devutils configure migrate", err=True)
            raise typer.Exit(1)

    if config_r3 is not None:
        typer.echo("Using saved configuration from config.yaml")
        enable_testing = config_r3["enable_testing"]
        enable_logenium_testing = config_r3["logenium"]["enable_testing"]
        enable_xheader_testing = config_r3["xheader"]["enable_testing"]
        enable_debug_testing = config_r3["debug"]["enable_testing"]
        enable_corelib_testing = config_r3["corelib"]["enable_testing"]
        enable_logging_testing = config_r3["logging"]["enable_testing"]
        mode = config_r3["build_mode"]

        # debug library options
        debug_use_fast_stacktrace = config_r3["debug"]["use_fast_stacktrace"]

        # corelib library options
        corelib_enable_tracing = config_r3["corelib"]["enable_tracing"]

    else:
        # === Build Mode ===
        typer.echo("")
        typer.echo(typer.style("=== Build Mode ===", fg="cyan", bold=True))
        typer.echo("Available modes: Debug, Release, RelWithDebInfo, MinSizeRel")
        mode_input: str = cast(str, typer.prompt("Select build mode"))
        mode_map = {
            "debug": "Debug",
            "release": "Release",
            "relwithdebinfo": "RelWithDebInfo",
            "minsizerel": "MinSizeRel",
        }

        if mode_input.lower() not in mode_map:
            typer.echo(typer.style("Invalid mode, defaulting to Release", fg=typer.colors.RED))
            mode = "Release"
        else:
            mode = mode_map[mode_input.lower()]
        typer.echo(f"  -> Build mode: {mode}")

        # === Debug Library Configuration ===
        typer.echo("")
        typer.echo(typer.style("=== Debug Library Configuration ===", fg="cyan", bold=True))
        typer.echo("The debug library provides Assert, Breakpoint, and profiling utilities.")
        typer.echo("")

        debug_use_fast_stacktrace = typer.confirm(
            "[debug] Use fast stacktrace? (prints stack in execution order instead of reversed)"
        )
        typer.echo(f"  -> Fast stacktrace: {'enabled' if debug_use_fast_stacktrace else 'disabled'}")

        # === Corelib Configuration ===
        typer.echo("")
        typer.echo(typer.style("=== Corelib Configuration ===", fg="cyan", bold=True))
        typer.echo("The corelib library provides casting utilities, RTTI, and type traits.")
        typer.echo("")

        corelib_enable_tracing = typer.confirm(
            "[corelib] Enable Tracy profiling instrumentation? (adds CRLB_ZONE_SCOPED to corelib functions)"
        )
        typer.echo(f"  -> Tracing: {'enabled' if corelib_enable_tracing else 'disabled'}")

        # === Testing Configuration ===
        typer.echo("")
        typer.echo(typer.style("=== Testing Configuration ===", fg="cyan", bold=True))
        typer.echo("Configure which test suites to build (uses GoogleTest).")
        typer.echo("")

        enable_testing = typer.confirm("[testing] Enable test builds?")
        enable_logenium_testing = False
        enable_xheader_testing = False
        enable_debug_testing = False
        enable_corelib_testing = False
        enable_logging_testing = False

        if enable_testing:
            typer.echo("  -> Testing: enabled")
            typer.echo("")

            enable_logenium_testing = typer.confirm("[testing] Build logenium tests? (Application/Window tests)")
            typer.echo(f"  -> Logenium tests: {'enabled' if enable_logenium_testing else 'disabled'}")

            enable_xheader_testing = typer.confirm("[testing] Build xheader tests? (Platform abstraction tests)")
            typer.echo(f"  -> Xheader tests: {'enabled' if enable_xheader_testing else 'disabled'}")

            enable_debug_testing = typer.confirm("[testing] Build debug library tests? (Assert/Breakpoint tests)")
            typer.echo(f"  -> Debug tests: {'enabled' if enable_debug_testing else 'disabled'}")

            enable_corelib_testing = typer.confirm("[testing] Build corelib tests? (Casting/RTTI/utility tests)")
            typer.echo(f"  -> Corelib tests: {'enabled' if enable_corelib_testing else 'disabled'}")

            enable_logging_testing = typer.confirm("[testing] Build logging library tests? (Logging tests)")
            typer.echo(f"  -> Logging tests: {'enabled' if enable_logging_testing else 'disabled'}")

        else:
            typer.echo("  -> Testing: disabled (all test suites skipped)")

        new_config: ConfigurationR3 = {
            "revision": 3,
            "enable_testing": enable_testing,
            "build_mode": mode,
            "logenium": {"enable_testing": enable_logenium_testing},
            "xheader": {"enable_testing": enable_xheader_testing},
            "debug": {
                "use_fast_stacktrace": debug_use_fast_stacktrace,
                "enable_testing": enable_debug_testing,
            },
            "corelib": {"enable_testing": enable_corelib_testing, "enable_tracing": corelib_enable_tracing},
            "logging": {"enable_testing": enable_logging_testing},
        }
        save_configuration_r3(new_config)
        typer.echo(f"Configuration saved to {ConfigFiles.config.relative_to(Directories.root)}")

    cmake_path = shutil.which("cmake")
    if cmake_path is None:
        typer.echo(typer.style("CMake is not installed", fg=typer.colors.RED))
        raise typer.Exit(1)

    command_line = [cmake_path]
    command_line.extend(["-S", str(Directories.root)])
    command_line.extend(["-B", str(Directories.build)])
    command_line.extend(["-DCMAKE_BUILD_TYPE=" + mode])

    # debug library options
    command_line.extend(["-DLOGENIUM_DEBUG_USE_FAST_STACKTRACE=" + ("ON" if debug_use_fast_stacktrace else "OFF")])

    # corelib library options
    command_line.extend(["-DLOGENIUM_CORELIB_ENABLE_TRACING=" + ("ON" if corelib_enable_tracing else "OFF")])

    # test options
    command_line.extend([f"-DLOGENIUM_BUILD_TESTS={'ON' if enable_testing else 'OFF'}"])
    command_line.extend([f"-DLOGENIUM_XHEADER_BUILD_TESTS={'ON' if enable_xheader_testing else 'OFF'}"])
    command_line.extend([f"-DLOGENIUM_DEBUG_BUILD_TESTS={'ON' if enable_debug_testing else 'OFF'}"])
    command_line.extend([f"-DLOGENIUM_CORELIB_BUILD_TESTS={'ON' if enable_corelib_testing else 'OFF'}"])
    command_line.extend(["-G", "Ninja"])

    command = shlex.join(command_line)
    typer.echo(f"Running command: {command}")
    subprocess.run(command_line, check=True)
    typer.echo("Done!")


@configure.callback(invoke_without_command=True)  # type: ignore[misc]
def main(
    ctx: typer.Context,
    reconfigure: bool = typer.Option(False, "--reconfigure", help="Remove saved configuration and reconfigure"),
) -> None:
    if ctx.invoked_subcommand is None:
        run(reconfigure=reconfigure)
