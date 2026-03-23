source_files=$(find src -type f -regex ".*.[cpp|h|cc]" | sed 's/^/\/project\//')
docker run --user $(id -u):$(id -g) -v .:/project xianpengshen/clang-tools:21-alpine clang-tidy --config-file=/project/scripts/.clang-tidy --verify-config &&
docker run --user $(id -u):$(id -g) -v .:/project xianpengshen/clang-tools:21-alpine clang-tidy --config-file=/project/scripts/.clang-tidy $source_files