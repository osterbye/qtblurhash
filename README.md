# QtBlurHash
Qt implementation of [BlurHash](https://github.com/woltapp/blurhash).

It's mostly based on the C implementation.

The encoder and decoder is wrapped in a shared library, for ease of use.
It is also possible to simply grab the _encoder.*_ or _decoder.*_ files, in case you only need one of them.

## Encoder
To use the encoder call the function:
```c++
const QVector<char> blurHashFromImage(int xComponents, int yComponents, const QImage *image, int scaleWidth);
```
* `xComponents`: Must be 1-9. A higher number gives a more detailed but longer BlurHash. It also takes longer to encode.
* `yComponents`: Must be 1-9. A higher number gives a more detailed but longer BlurHash. It also takes longer to encode.
* `image`: A pointer to a valid QImage. The QImage must remain valid for the entire encoding.
* `scaleWidth` (optional): This will scale the _image_ to the given width, keeping aspect ratio, before doing the encoding. For large images scaling down will improve encoding time significantly and will create BlurHashes of pretty much the same quality. _image_ will only be scaled if the original width is larger than the supplied value. 

## Decoder
To use the decoder call the function:
```c++
QImage imageFromBlurHash(QByteArray *blurhash, int width, int height, int punch);
```
* `blurhash`: A pointer to a QByteArray containing the BlurHash. The QByteArray must remain valid for the entire decoding.
* `width`: Width of the blurred output image.
* `height`: Height of the blurred output image.
* `punch` (optional): Adjusts the contrast of the output image. Tweak it if you want a different look for your placeholders.

## How to build as a library
Build the project in the subfolder _qtblurhash/_.
```bash
qmake qtblurhash.pro
make -s
```
Your project should include the library (obviously) and the header file _qtblurhash.h_ wherever you want to use it.

## How to use source files
If you want to use the source files directly in your own project, just copy _common.h_ as well as either the _encoder.*_ or _decoder.*_ files, depending on what you need.

## Example application
An example application, using the library, can be found in the subfolder _example/_. The application is a console application, so run it from a console. \
In order to build both the library and the example application, simply build the _example\_app.pro_ project, from the root of the repository.
```bash
qmake example_app.pro
make -s
```
The binaries will appear in a _bin/_ folder. From the _bin/_ folder, you can run the console application. \
You might need to set the _LD\_LIBRARY\_PATH_ of your environment, in order for the application to find the library.
```bash
cd bin
export LD_LIBRARY_PATH=$PWD:${LD_LIBRARY_PATH}
```
You also need an image to perform the encoding on. The example app supports input in all the image formats that your Qt version supports.

To create a hash, from an image:
```bash
./test_app 9 6 mypic.jpg hash.txt
```
The x and y must be between 1-9.

To create a blurred image, from a hash:
```bash
./test_app 64 48 hash.txt myblurredpic.jpg
```
The example app supports output in all the image formats that your Qt version supports.

