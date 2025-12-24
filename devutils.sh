#!/bin/bash
# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

set -e

# Check if uv is installed
if command -v uv &> /dev/null; then
    echo -e "\033[0;32mFound uv\033[0m"
else
    echo -e "\033[0;33muv not found, installing it now\033[0m"
    echo -e "\033[0;33mThis may take a while...\033[0m"

    # Install uv
    curl -LsSf https://astral.sh/uv/install.sh | sh

    # Source the updated PATH (uv is installed to ~/.local/bin by default)
    export PATH="$HOME/.local/bin:$PATH"

    # Install Python 3.14
    uv python install 3.14
fi

# Create virtual environment if it doesn't exist
if [ ! -d ".venv" ]; then
    echo -e "\033[0;33mCreating virtual environment...\033[0m"
    uv venv
    echo -e "\033[0;33mInstalling requirements...\033[0m"
    uv pip install -r requirements.txt
else
    echo -e "\033[0;32mVirtual environment found, skipping setup\033[0m"
fi

# Run devutils with all arguments
uv run devutils "$@"
