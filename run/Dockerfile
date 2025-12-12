
FROM debian:bookworm-slim

ENV TZ=UTC \
    LANG=C

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    gcc-multilib \
    binutils \
    make \
    gdb \
    zsh \
    bash \
    vim \
    ca-certificates \
 && rm -rf /var/lib/apt/lists/*

WORKDIR /app

