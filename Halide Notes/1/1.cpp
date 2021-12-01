#include "Halide.h"

// Include some support code for loading pngs.
#include "halide_image_io.h"
using namespace Halide::Tools;
#include<iostream>

void brightManual(int factor){
    Halide::Buffer<uint8_t> input = load_image("futurama.jpg");
    int h = input.height();
    int w = input.width();
    int c = input.channels();
    Halide::Buffer<uint8_t> output(w, h, c);
    uint8_t *inbuf = input.get()->data();
    uint8_t *outbuf = output.get()->data();

 
    for(int j=0;j<h;j++){
        for(int k=0;k<w;k++){

            *(outbuf + 3*j*w + 3*k) = std::clamp(*(inbuf + 3*j*w + 3*k)+factor,0,255);
            *(outbuf + 3*j*w + 3*k + 1 ) =  std::clamp(*(inbuf + 3*j*w + 3*k + 1)+factor,0,255);
            *(outbuf + 3*j*w + 3*k + 2) =  std::clamp(*(inbuf + 3*j*w + 3*k + 2)+factor,0,255);
            // *(outbuf + j*w + k) = std::clamp(*(inbuf + j*w + k)+factor,0,255);
            // *(outbuf + (h*w)+ j*w + k ) =  std::clamp(*(inbuf + (h*w)+ j*w + k)+factor,0,255);
            // *(outbuf + (2*h*w) + j*w + k) =  std::clamp(*(inbuf + (2*h*w) + j*w + k)+factor,0,255);
            
        }
    }

    save_image(output,"manual.jpg");

};

void brightHalide(int factor){
    Halide::Buffer<uint8_t> input = load_image("futurama.jpg");
    Halide::Func brighter;
    Halide::Var x, y, c;

    Halide::Expr value = input(x, y, c);

    value = Halide::cast<int>(value);

    
    value = value + factor;
    value = Halide::min(value, 255.0f);

    value = Halide::cast<uint8_t>(value);

    brighter(x, y, c) = value;
    Halide::Buffer<uint8_t> output =
        brighter.realize({input.width(), input.height(), input.channels()});

    save_image(output, "brighterHalide.png");


};
int main(int argc, char **argv) {

    // This program defines a single-stage imaging pipeline that
    // brightens an image.
    brightManual(11);
    brightHalide(11);
    
     

    printf("Success!\n");
    return 0;
     
}
