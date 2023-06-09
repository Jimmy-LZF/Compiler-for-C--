#!/bin/bash

# 生成解析器代码
bison -d syntax.y
flex lexical.l
gcc main.c syntax.tab.c -lfl -o parser

# 指定要查找的目录
dir_path="Tests"

# 创建结果文件夹
mkdir -p "result"

# 查找所有 .cmm 文件并处理它们
while IFS= read -r file; do
    # 从文件路径中提取文件名和扩展名
    filename=$(basename -- "$file")
    extension="${filename##*.}"
    filename="${filename%.*}"

    # 处理文件内容
    ./parser "$file" > "result/$filename.output"
    ./parser "$file"
done < <(find "$dir_path" -type f -name "*.cmm")