# I - Halide Basics

In this tutorial we will walk over the basics of Halide operations by creating a simple program that brigtens images. This will also introduce Func, Var and Expr objects


## 0- Importing libraries

Gotta get them libraries first!. 

```{.cpp}
#include "Halide.h"
#include "halide_image_io.h"
using namespace Halide::Tools;
#include<iostream>

```

"Halide.h" header contains everything you will need for Halide library. "halide_image_io.h" contains utility method for loading and saving images.

## 1- Creating a Simple Brigtening program

We will now write a basic function to brighten an image pixel-by-pixel. It is pretty rudementry as it is composed of bunch of for loops but it does the job. 

It will also introduce you to Halide's loading and saving functionality for images.

```{.cpp}
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

```

Notice, that I have 3 lines commented. RGB images can come in two main types of layout in memory - Planar and Interleaved. 

Planar images have the red, green, and blue channels laid out in memory like this:

RRRRRRRR
GGGGGGGG
BBBBBBBB

For interleaved format red, green, and blue values for each pixel occur next to each other in memory:

RGBRGBRGBRGBRGBRGBRGBRGB


In addition,

* **load_image()** is used to open input images 
* The image is saved as using the **save_image()** function.
* The **get()->data()** method is used to get pointer to actual data.  


<br />

## 2- Brigtening Operation

The operation being formed is pretty simple where we literally just add a factor to each pixel value. We just need to make sure to clamp the values between 0-255.


<br />


## 3- Func, Vars, Expression

**Func** represents one stage in a Halide pipeline, and is the unit by which we schedule things. The Func object is the only shedulable object in Halide. Func and Expre contain manipulations of Vars. 

**Vars** objects are names to use as variables in the definition of a Func. They have no meaning by themselves.

**Expr** objects are used to determine the flow of computation (acted on Vars). According to official tutorials, Funcs are defined at any integer coordinate of its variables as an Expr in terms of those variables and other functions.

IMHO, all the distinction will be clean when we look at a bunch of examples.

<br />

## 4- Halidifying the Brigtening operation

Finally lets look at what the Halide implementation of the above code looks like.

```{.cpp}
void brightHalide(int factor){
    Halide::Buffer<uint8_t> input = load_image("futurama.jpg");
    
    Halide::Func brighter;
    Halide::Var x, y, c;

    Halide::Expr value = input(x, y, c);

    value = value + factor;
    value = Halide::min(value, 255.0f);

    value = Halide::cast<uint8_t>(value);

    brighter(x, y, c) = value;
    Halide::Buffer<uint8_t> output =
        brighter.realize({input.width(), input.height(), input.channels()});

    save_image(output, "brighterHalide.png");

    
};
```

In this function we first define a **Func** 'brighter' and **Var** associated with spatial pixels and channels. 
**Funcs** are defined at any integer coordinate of its variables as an **Expr** in terms of those variables and other functions.
Here, we'll define an Expr which has the value input(x,y,z) which is the pixel value.

We add the desired factor to it and clamp and cast it to int8. 

     brighter(x, y, c) = value;

This line of code defined the sets the Fun in terms of the Expr values, but it didn't actually compute the output image yet. At this stage it's just Funcs,Exprs, and Vars in memory, representing the structure of our imaging pipeline. We're meta-programming. This C++ program is constructing a Halide program in memory. Actually computing pixel data comes next.

    Halide::Buffer<uint8_t> output =brighter.realize({input.width(), input.height(), input.channels()});

'realize'-ing the Func JIT compiles it and implements the pipeline we've defined, and then runs it.  We also need to tell Halide the domain over which to evaluate the Func, which determines the range of x and y above, and the resolution of the output image. 

## 5 - References

* https://halide-lang.org/tutorials/tutorial_lesson_02_input_image.html
* https://halide-lang.org/tutorials/tutorial_lesson_01_basics.html


