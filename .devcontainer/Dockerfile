FROM mcr.microsoft.com/vscode/devcontainers/base:ubuntu

USER root

RUN DEBIAN_FRONTEND=noninteractive \
    apt-get update && \
    apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    clang-format \
    gdb \
    libicu-dev \
    libgtest-dev

RUN useradd -m hemiola

USER hemiola