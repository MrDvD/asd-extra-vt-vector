source_files=$(find src -type f -regex ".*.[cpp|hpp|h|cc]" | grep -v '^src/tests' | sed 's/^/\/project\//')
sed -i "s|$(pwd)|/project|g" build/compile_commands.json
docker run -v .:/project xianpengshen/clang-tools:21 clang-tidy --config-file=/project/scripts/.clang-tidy -p /project/build --verify-config &&
docker run -v .:/project xianpengshen/clang-tools:21 clang-tidy --config-file=/project/scripts/.clang-tidy -p /project/build $source_files