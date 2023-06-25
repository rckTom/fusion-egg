#include "app.h"
#include "zauberstab.h"

#define __stringify_1(x)	#x
#define __stringify(x)		__stringify_1(x)

extern "C"
{
#define BASE_DIR "C:/Users/Binarykitchen/Documents/tom/fusion-zauberstab/firmware/"
asm(
".macro inc_sample name, filename\n\t"
        ".pushsection .rodata\n\t"
        "\\name:\n\t"
                ".incbin \"\\filename\"\n\t"
        "\\name\\()_size:\n\t"
	".int \\name\\()_size - \\name\n\t"
        ".popsection\n\t"
".endm\n\t"
);

#define incbin(label, filename) \
	asm("inc_sample " __stringify(label) ", " filename "\n\t"); \
	extern const unsigned char label[]; \
	extern const unsigned int label##_size; \



incbin(fusion_font, BASE_DIR "assets/fairydust.bin")
}

static CRGB get_pixel(unsigned int x, unsigned int y, unsigned int sx, const unsigned char *data) {
    unsigned int idx = (x + y * sx) * 3;

    CRGB color{};
    color.r = data[idx];
    color.g = data[idx + 1];
    color.b = data[idx + 2];

    return color;
}

void ImageDisplayApp::init() {

}

void ImageDisplayApp::deinit() {

}

void ImageDisplayApp::loop() {
    static unsigned int col = 0;
    unsigned int sx = fusion_font_size/(3*48);

    for (int i = 0; i< NUM_LEDS; i++) {
        leds[i] = get_pixel(col, i, sx, fusion_font);
    }
    col++;
    col = col % 218;
    FastLED.show();
}