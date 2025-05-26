#!/bin/bash

projectName="MMO"
projectDir="/home/wil/dev/sro/game"
engineDir="/home/wil/dev/ue5/Engine"

project="$projectDir/$projectName.uproject"
buildScript="$engineDir/Binaries/DotNET/UnrealBuildTool/UnrealBuildTool"

replaceStr="clang++\"\,\n\t\t\t\"@$projectDir/clang-flags.txt\""


$buildScript \
  -mode=GenerateClangDatabase \
  -project=$project \
  -game \
  -vscode \
  -makefile \
  -engine MMOEditor Linux Development

rm "$projectDir/compile_commands.json"
ln -s "$projectDir/.vscode/compileCommands_$projectName.json" "$projectDir/compile_commands.json"
sed -i -e "s,$engineDir\(.*\)clang++,$replaceStr,g" compile_commands.json
ctags -R Source

