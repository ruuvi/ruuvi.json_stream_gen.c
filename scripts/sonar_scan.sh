#!/bin/bash

set -e # Exit script on non-zero command exit status
set -x # Print commands and their arguments as they are executed.

export SONAR_TOKEN=$SONAR_TOKEN_ruuvi_json_stream_gen
if [ "$SONAR_TOKEN" = "" ]; then
     echo Environment variable "SONAR_TOKEN_ruuvi_json_stream_gen" is not set
     exit 1
fi

mkdir -p "$HOME/bin"
if [ ! -d "$HOME/bin/sonar-scanner" ]; then
  rm -f "$HOME/bin/sonar-scanner-cli-4.7.0.2747-linux.zip"
  wget -P "$HOME/bin" "https://binaries.sonarsource.com/Distribution/sonar-scanner-cli/sonar-scanner-cli-4.7.0.2747-linux.zip"
  unzip -q -o "$HOME/bin/sonar-scanner-cli-4.7.0.2747-linux.zip" -d "$HOME/bin"
  mv "$HOME/bin/sonar-scanner-4.7.0.2747-linux" "$HOME/bin/sonar-scanner"
fi
export PATH=$PATH:$HOME/bin/sonar-scanner/bin

rm -f "$HOME/bin/build-wrapper-linux-x86.zip"
wget -P "$HOME/bin" https://sonarcloud.io/static/cpp/build-wrapper-linux-x86.zip
unzip -o "$HOME/bin/build-wrapper-linux-x86.zip" -d "$HOME/bin"
export PATH=$PATH:$HOME/bin/build-wrapper-linux-x86

rm -rf build
mkdir -p build
cd build
which build-wrapper-linux-x86-64
build-wrapper-linux-x86-64 --out-dir ./bw-output cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS="--coverage" ..
build-wrapper-linux-x86-64 --out-dir ./bw-output make -j $(nproc)
cd ..

cd tests
rm -rf build
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS="--coverage" ..
make -j $(nproc)
ctest
cd ../..
gcovr -r . --sonarqube -o coverage.xml

which sonar-scanner
sonar-scanner --version
sonar-scanner -X \
  --define sonar.cfamily.build-wrapper-output="build/bw-output" \
  --define sonar.coverageReportPaths=coverage.xml \
  --define sonar.host.url=https://sonarcloud.io \
  --define sonar.cfamily.threads=$(nproc)
