# example-signature
Example how to calculate signature of a very big file.

The main idea is to use memory mapped files, because
they suit that purpose perfectly - you don't need to
manage file chunks manually, all that stuff will be
done by the OS.

## How to build
1. `mkdir ../example-signature-build`
2. `cd ../example-signature-build`
3. `conan install ../example-signature -s compiler.libcxx=libstdc++11 --build=boost` -
there are some Linux specific flags, but should be possible to replicate on Windows.
4. `cmake ../example-signature`
5. `cmake --build .`
6. Now there will be `bin` folder where you can find binary & tests binary. Tests binary
expect that working directory will be `tests/data` (yeah, a little bit inconvenient).

## Usage
`example-signature input_file output_file 1024`

## Known limitations which can be improved
1. Output file will be executable.
2. Input file cannot be empty.
3. Argparsing isn't done properly.
