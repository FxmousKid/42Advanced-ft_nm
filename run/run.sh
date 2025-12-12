#!/bin/bash

set -e

CONTAINER_NAME="ft_nm"
DOCKER_IMAGE="ft_nm-debian"
PROJECT_DIR="$PWD"
DOCKERFILE_PATH="$PROJECT_DIR/run/Dockerfile"

TESTFILE="$PROJECT_DIR/test/test.c"

command="make test"

# Show help
show_help() {
	cat << EOF
Usage: ./$0 [options]

Options:
  -h        show usage
  -k        Kill the $CONTAINER_NAME container
  -b        Rebuilds the Docker images (forces update) and runs 
            the code (! building is Needed on 1st run)
  -B        Same as -b, but exits after rebuilding

Examples:
  $0
EOF
}

# Build Docker image with QEMU emulation
build_images() {
	echo "[*] Enabling QEMU binfmt emulation..."
	docker run --rm --privileged tonistiigi/binfmt --install all

	echo "[*] Building Docker image ($DOCKER_IMAGE)..."
	docker buildx build --platform linux/amd64 \
		-t "$DOCKER_IMAGE" \
		-f "$DOCKERFILE_PATH" \
		"$PROJECT_DIR"
}

# Run the built Docker image
run_image() {
	docker run --rm -it \
		--platform linux/amd64 \
		--name "$CONTAINER_NAME" \
		-v "$PROJECT_DIR:/app" \
		-w /app \
		--network="host" \
		"$DOCKER_IMAGE" \
		zsh -c "$command"
}

# Parse flags
while getopts ":hbBk" opt; do
	case $opt in
		b)
			build_images
			;;
		B)
			build_images
			exit 0
			;;
		h)
			show_help
			exit 0
			;;
		k)
			docker stop $CONTAINER_NAME
			exit 0
			;;
		\?)
			echo "$0: invalid option -- '$OPTARG'"
			echo "Try '$0 -h' for more information."
			exit 0
			;;
	esac
done
shift $((OPTIND - 1))

run_image
