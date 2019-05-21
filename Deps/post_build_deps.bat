@echo off

pushd .

cd "%~dp0"
echo Current directory is %cd%

if NOT EXIST ./zlib/zconf.h (
    echo "Moving zconf.h.included -> zconf.h if needed, so Git is happy about Deps/zlib being clean"
    ren ./zlib/zconf.h.included ./zlib/zconf.h
)

popd

echo "Script is done"
