FROM mcr.microsoft.com/vscode/devcontainers/base:ubuntu

USER root

RUN DEBIAN_FRONTEND=noninteractive \
    apt update && \
    apt install -y \
    build-essential \
    cmake \
    clang-format \
    gdb \
    libicu-dev \
    libgtest-dev