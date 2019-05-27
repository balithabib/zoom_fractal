#include "fractal.hpp"
#include <iostream>
using namespace cv;
//======================================Les constructeurs=======================================
Fractal::Fractal(): w_(800), h_(512), nbr_thread_(8), N_ITER_(300), zoom_(0.002f), step_x_(-0.05100951f), step_y_(-0.5200008f)
{
    img = Mat(h_, w_,  CV_8UC (3), Scalar(0));
    C = std::complex<long double>(0.32,  0.43);

    thread_fractal();

}
Fractal::Fractal(int w, int h, int nbr_thread, int nbr_iter,long double zoom, long double step_x, long double step_y, long double x, long double y):
    w_(w), h_(h), nbr_thread_(nbr_thread), N_ITER_(nbr_iter), zoom_(zoom), step_x_(step_x), step_y_(step_y)
{
    img = Mat(h_, w_,  CV_8UC (3), Scalar(0));
    C = std::complex<long double>(y, x);
    color[0]=0.5; color[1]=0.5; color[2]=0.5;
    color[3]=0.5; color[4]=0.5; color[5]=0.5;
    color[6]=1.0; color[7]=1.0; color[8]=1.0;
    color[9]=0.0; color[10]=0.10; color[11]=0.20;
    /*color[0]=0.5; color[1]=0.5; color[2]=0.5;
    color[3]=0.5; color[4]=0.5; color[5]=0.5;
    color[6]=1.0; color[7]=1.0; color[8]=1.0;
    color[9]=0.30; color[10]=0.20; color[11]=0.20;*/
    /*color[0]=0.5; color[1]=0.5; color[2]=0.5;
    color[3]=0.5; color[4]=0.5; color[5]=0.5;
    color[6]=1.0; color[7]=0.7; color[8]=0.4;
    color[9]=0.00; color[10]=0.15; color[11]=0.20;*/
    thread_fractal();

}
//================================================================================================
Vec3b Fractal::mandelbrot_pix(long double x, long double y) {

    std::complex<long double> c(y, x);
    std::complex<long double> z(0, 0);
    for(int i = 0; i < N_ITER_; ++i) {
        if(norm(z) > THRESHOLD_) {
            return coloring(i);
        }
        z = z * z + c;
    }   
    return Vec3b(0, 0, 0);
}

Vec3b Fractal::julia_pix(long double x, long double y) {
    std::complex<long double> z(y, x);
    for(int i = 0; i < N_ITER_; ++i) {
        if(norm(z) > THRESHOLD_) {
            return coloring(i);
        }
        z = z * z + C;
    }       
    return Vec3b(0, 0, 0);
}

void Fractal::julia(int begin_r, int end_r)
{
    Vec3b color;
    for (int y = begin_r; y < end_r; y++){
        //changement de variable par rapport à l'axe des y.
        long double real = - ((long double)y - img.rows / 2.0) * zoom_ + step_y_;
        for (int x = 0; x < img.cols; x++){
            //changement de variable par rapport à l'axe des x.
            long double imag = ((long double)x - img.cols / 2.0) * zoom_ + step_x_;
            //calcule de la couleur pour le pixel actuelle.
            img.at<Vec3b>(y, x) = julia_pix(real,imag);
        }
    }
    //dessiner un point rouge sur le centre de zoom qui correspond au centre de l'image. 
    int hh = img.rows / 2;
    int ww = img.cols / 2;
    img.at<Vec3b>(hh - 1, ww - 1) = Vec3b(0, 0, 255);
    img.at<Vec3b>(hh - 1, ww) = Vec3b(0, 0, 255);
    img.at<Vec3b>(hh - 1, ww + 1) = Vec3b(0, 0, 255);
    img.at<Vec3b>(hh, ww - 1) = Vec3b(0, 0, 255);
    img.at<Vec3b>(hh, ww) = Vec3b(0, 0, 255);
    img.at<Vec3b>(hh, ww + 1) = Vec3b(0, 0, 255);
    img.at<Vec3b>(hh + 1, ww - 1) = Vec3b(0, 0, 255);
    img.at<Vec3b>(hh + 1, ww) = Vec3b(0, 0, 255);
    img.at<Vec3b>(hh + 1, ww + 1) = Vec3b(0, 0, 255);
}

void Fractal::thread_fractal()
{
    int step_r = img.rows/nbr_thread_; //le step des lignes (nombre de ligne par thread).
    int begin_r = 0;    //(l'indice de debut de ligne).
    int end_r = step_r; //(l'indice de fin de ligne).
   
    for (int r = 0;r < nbr_thread_; ++r)
    {
        auto m1 = std::async(std::launch::async, &Fractal::julia, this, begin_r, end_r);
        // actualisation des indices lignes.
        begin_r = end_r;
        end_r += step_r;         
    }
    // appele au thread
    for (auto &th : thread_vec_){
        th.get();
    }
}

void Fractal::zoom_fractal_resize(int mode)
{
    int p = 2, fps = 60, x , y, w_dist, h_dist, k = 0;
    float scale;

    img = Mat(p * h_, p * w_,  CV_8UC (3), Scalar(0));
    
    VideoWriter video("zoom_fractal_julia.avi", CV_FOURCC('M','J','P','G'), fps, Size(w_,h_), true);
    
    fps = 120;
    for (int i = 0; i < 20; ++i) {
        // Calcule de la nouvel fractale on utilisant les thread ou on sans les threads.
        if(mode){
            thread_fractal();
        }else{
            julia(0, img.rows);
        }
        //julia(0, img.rows);
        printf("rendu %d  N_ITER: %d  zoom :%.20Lf\n", i, N_ITER_, zoom_);
        // ici on resize fps = 120 fois la fractale calculer avant. 
        for(int j = fps; j >= 0; --j){
            // image resizer.
            Mat img_dist;
            // l'echel de réduction.
            scale =  1 + (float)j / (float)fps;
            //la taille de l'image resiezer. 
            w_dist =img.cols / scale;
            h_dist =img.rows / scale;
            resize(img, img_dist, Size(w_dist, h_dist), 0, 0, INTER_LINEAR);
            //on recuperer de l'image resizer une partie de taille (h, w).
            x = (w_dist/2) - w_/2;
            y = (h_dist/2) - h_/2;
            Mat img_screen = Mat(img_dist, Rect(x, y, w_, h_));

            imshow("fractal", img_screen);
            cvWaitKey(1);
            //ecrire la frame dans la video.
            video.write(img_screen);
            k++;
        }
        // actualisation du zoom.
        zoom_ /= 2.0;//249;
        // actualisation du nombre d'iteration.
        N_ITER_ += 128;
    }
   
    video.release();
}

void Fractal::fractal_parade()
{
    C = std::complex<long double>(0.0, 0.0);
    img = Mat(h_, w_,  CV_8UC (3), Scalar(0));
    VideoWriter video("zoom_fractal_julia.avi", CV_FOURCC('M','J','P','G'), 60, Size(w_,h_), true);
    while(true) {
            thread_fractal();
            imshow("fractal", img);
            setMouseCallback("fractal", (MouseCallback)&Fractal::on_mouse, &C);
            cvWaitKey(1);
    }
    video.release();
}

void Fractal::show(){
    img = Mat(h_, w_,  CV_8UC (3), Scalar(0));
    while(true) {
        thread_fractal();
        imshow("fractal", img);
        setMouseCallback("fractal", (MouseCallback)&Fractal::on_mouse_bis, &color);
        cvWaitKey(1);
    }
}
//=========================================Les set=========================================
void Fractal::set_zoom(long double zoom){
    zoom_ = zoom;
}

void Fractal::set_nbr_iter(int nbr_iter){
    N_ITER_ = nbr_iter;
}

float Fractal::palette(float t,float a, float b, float c, float d){
    return 255.0*(a + b * cos(6.28318*(c*t+d)));
}

Vec3b Fractal::coloring(int i){
    float red,green,blue,t = i/float(N_ITER_);
    red = palette(t,color[0],color[3],color[6],color[9]);
    green = palette(t,color[1],color[4],color[7],color[10]);
    blue = palette(t,color[2],color[5],color[8],color[11]);
    return Vec3b(blue,green,red);
}
//======================================Les callbacks=======================================
void Fractal::on_mouse(int event, int x, int y, int flags, void* data){
    if(event == 0){
        long double real = -((long double)y - 512.0 / 2.0) / (512.0 / 2.0);
        long double imag = ((long double)x - 800.0 / 2.0) / (800.0 / 2.0);

        //std::cout << "x : "<< real << " y: "<< imag <<"\n"; 
        std::complex<long double>&tmp = *((std::complex<long double>*)(data));
        tmp = std::complex<long double>(real, imag);
    }
}

void Fractal::on_mouse_bis(int event, int x, int y, int flags, void* data){
    float *tmp = ((float*) data);
    if(event == 1){
        int random = rand()%7 + 1;
        printf("%d\n", random);
        switch(random){
            case 1:{
                tmp[0]=0.5; tmp[1]=0.5; tmp[2]=0.5;
                tmp[3]=0.5; tmp[4]=0.5; tmp[5]=0.5;
                tmp[6]=1.0; tmp[7]=1.0; tmp[8]=1.0;
                tmp[9]=0.0; tmp[10]=0.33; tmp[11]=0.67;
            }break;
            case 2:{
                tmp[0]=0.5; tmp[1]=0.5; tmp[2]=0.5;
                tmp[3]=0.5; tmp[4]=0.5; tmp[5]=0.5;
                tmp[6]=1.0; tmp[7]=1.0; tmp[8]=1.0;
                tmp[9]=0.0; tmp[10]=0.10; tmp[11]=0.20;
            }break;
            case 3:{
                tmp[0]=0.5; tmp[1]=0.5; tmp[2]=0.5;
                tmp[3]=0.5; tmp[4]=0.5; tmp[5]=0.5;
                tmp[6]=1.0; tmp[7]=1.0; tmp[8]=1.0;
                tmp[9]=0.30; tmp[10]=0.20; tmp[11]=0.20;
            }break;
            case 4:{
                tmp[0]=0.5; tmp[1]=0.5; tmp[2]=0.5;
                tmp[3]=0.5; tmp[4]=0.5; tmp[5]=0.5;
                tmp[6]=1.0; tmp[7]=1.0; tmp[8]=0.5;
                tmp[9]=0.80; tmp[10]=0.90; tmp[11]=0.30;
            }break;
            case 5:{
                tmp[0]=0.5; tmp[1]=0.5; tmp[2]=0.5;
                tmp[3]=0.5; tmp[4]=0.5; tmp[5]=0.5;
                tmp[6]=1.0; tmp[7]=0.7; tmp[8]=0.4;
                tmp[9]=0.00; tmp[10]=0.15; tmp[11]=0.20;
            }break;
            case 6:{
                tmp[0]=0.5; tmp[1]=0.5; tmp[2]=0.5;
                tmp[3]=0.5; tmp[4]=0.5; tmp[5]=0.5;
                tmp[6]=2.0; tmp[7]=1.0; tmp[8]=0.0;
                tmp[9]=0.50; tmp[10]=0.20; tmp[11]=0.25;
            }break;
            case 7:{
                tmp[0]=0.8; tmp[1]=0.5; tmp[2]=0.4;
                tmp[3]=0.2; tmp[4]=0.4; tmp[5]=0.2;
                tmp[6]=2.0; tmp[7]=1.0; tmp[8]=1.0;
                tmp[9]=0.00; tmp[10]=0.25; tmp[11]=0.25;
            }break;
        }
    }
}
//===========================================================================================
