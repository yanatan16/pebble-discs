# discs

This is a [pebble](http://getpebble.com) watchface which uses a minimalist set of 4 overlapping discs to show analog time.

_Note_: This watchface is built for pebble 2.0 and will be available when that is released soon.

## Previews

![Preview image at noon](https://github.com/yanatan16/pebble-discs/blob/master/resources/preview_noon.png?raw=true)

![Preview image at 10:30](https://github.com/yanatan16/pebble-discs/blob/master/resources/preview_tenthirty.png?raw=true)

## How to build

Make sure you setup the SDK according to [pebble's instructions](http://developer.getpebble.com). Then clone this repo (or your fork) into your pebble directory. Then add the symlinks.

```
cd ~/pebble-dev
git clone http://github.com/yanatan16/pebble-discs
cd pebble-discs
pebble build
pebble install --phone <your_phone_ip>
```

## License

Copyright (c) 2013 Jon Eisen

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
