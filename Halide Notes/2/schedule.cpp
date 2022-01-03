#include "Halide.h"
#include "halide_image_io.h"
#include<iostream>
#include <chrono>
using namespace std::chrono;
using namespace Halide::Tools;

void brightOriginal(int factor){
    
    
    Halide::Buffer<uint8_t> input = load_image("futurama.jpg");

    //Start timing
    auto start = high_resolution_clock::now();
    Halide::Func brighter;
    Halide::Var x("x"), y("y"), c("c");
    Halide::Expr value = input(x, y, c);
    value = Halide::cast<int>(value);
    value = value + factor;
    value = Halide::min(value, 255.0f);
    value = Halide::cast<uint8_t>(value);
    brighter(x, y, c) = value;
    Halide::Buffer<uint8_t> output =
        brighter.realize({input.width(), input.height(), input.channels()});

    //Stop timing
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout<<"-----------------------------------------"<<std::endl;
    std::cout <<"Original "<< duration.count() << std::endl;
    brighter.print_loop_nest();
    
    std::cout<<"-----------------------------------------"<<std::endl;
};

void brightReorder(int factor){
    
    
    Halide::Buffer<uint8_t> input = load_image("futurama.jpg");
    auto start = high_resolution_clock::now();
    Halide::Func brighter;
    Halide::Var x("x"), y("y"), c("c");
    Halide::Expr value = input(x, y, c);
    value = Halide::cast<int>(value);
    value = value + factor;
    value = Halide::min(value, 255.0f);
    value = Halide::cast<uint8_t>(value);
    brighter(x, y, c) = value;
    
    // ----------- Begin ----------------
    brighter.reorder(c,x,y);
    // -----------End---------------------
    
    
    Halide::Buffer<uint8_t> output =
        brighter.realize({input.width(), input.height(), input.channels()});


    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout<<"-----------------------------------------"<<std::endl;
    std::cout <<"Reorder "<< duration.count() << std::endl;
    brighter.print_loop_nest();
    
    std::cout<<"-----------------------------------------"<<std::endl;
};

void brightTiled(int factor){
    
    
    Halide::Buffer<uint8_t> input = load_image("futurama.jpg");
    auto start = high_resolution_clock::now();
    Halide::Func brighter;
    Halide::Var x("x"), y("y"), c("c");

    Halide::Expr value = input(x, y, c);
    value = Halide::cast<int>(value);    
    value = value + factor;
    value = Halide::min(value, 255.0f);

    value = Halide::cast<uint8_t>(value);
    Halide::Var x_outer("x_outer"), x_inner("x_inner"), y_outer("y_outer"), y_inner("y_inner");
    brighter(x, y, c) = value;

    // ----------- Begin ----------------
    brighter.tile(x, y, x_outer, y_outer, x_inner, y_inner, 4, 4);
    brighter.reorder(c,x_inner,y_inner,x_outer,y_outer);
    // -----------End---------------------

    Halide::Buffer<uint8_t> output =
        brighter.realize({input.width(), input.height(), input.channels()});
    

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout<<"-----------------------------------------"<<std::endl;
    std::cout <<"Reorder "<< duration.count() << std::endl;
    brighter.print_loop_nest();
    
    std::cout<<"-----------------------------------------"<<std::endl;
};

void brightVector(int factor){
    
   
    Halide::Buffer<uint8_t> input = load_image("futurama.jpg");
     auto start = high_resolution_clock::now();
    Halide::Func brighter;
    Halide::Var x("x"), y("y"), c("c");
    Halide::Expr value = input(x, y, c);
    value = Halide::cast<int>(value);   
    value = value + factor;
    value = Halide::min(value, 255.0f);
    value = Halide::cast<uint8_t>(value);
    Halide::Var x_outer("x_outer"), x_inner("x_inner"), y_outer("y_outer"), y_inner("y_inner");
    brighter(x, y, c) = value;
    Halide::Var x_vo("x_vo"), x_vi("x_vi");

    // ----------- Begin ----------------
    brighter.split(x, x_vo, x_vi, 4).vectorize(x_vi).parallel(c);
    // -----------End---------------------
    
    Halide::Buffer<uint8_t> output =
        brighter.realize({input.width(), input.height(), input.channels()});
    


    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout<<"-----------------------------------------"<<std::endl;
    std::cout <<"Vector "<< duration.count() << std::endl;
    brighter.print_loop_nest();
    
    std::cout<<"-----------------------------------------"<<std::endl;
};




int main(int argc, char **argv) {

    brightOriginal(11);
    brightReorder(11);
    brightTiled(11);
    brightVector(11);
     
    return 0;
     
}
