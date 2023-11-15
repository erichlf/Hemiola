FROM mcr.microsoft.com/vscode/devcontainers/base:ubuntu

USER root

RUN DEBIAN_FRONTEND=noninteractive \
    apt-get update && \
    apt-get install -y --no-install-recommends \
    build-essential=12.9ubuntu3 \
    cmake=3.22.1-1ubuntu1.22.04.1 \
    clangd=1:14.0-55~exp2 \
    clang-format=1:14.0-55~exp2 \
    gdb=12.1-0ubuntu1~22.04 \
    libicu-dev=70.1-2 \
    libgtest-dev=1.11.0-3

RUN useradd -m hemiola

USER hemiola