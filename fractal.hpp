#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <vector>
#include <thread>
#include <future>
#include <complex>

using namespace cv;

class Fractal {
public:
    Fractal();
    Fractal(int w, int h, int nbr_thread = 8, int nbr_iter = 300,long double zoom = 0.002f, long double step_x = 0.0f, long double step_y = 0.0f, long double x = -0.1610, long double y =  -0.7927);

    Vec3b mandelbrot_pix(long double x, long double y); 
    Vec3b julia_pix(long double x, long double y);

    void julia(int begin_r, int end_r);
    void thread_fractal(); 
    void zoom_fractal_resize(int mode = 1);
    void fractal_parade();
    void show();
    void set_zoom(long double zooom);
    void set_nbr_iter(int nbr_iter);
    float palette(float t,float a, float b, float c, float d);
    Vec3b coloring(int i);

    static void on_mouse(int event, int x, int y, int flags, void* data);
    static void on_mouse_bis(int event, int x, int y, int flags, void* data);
private:
    std::vector<std::future<int>> thread_vec_;
    std::complex<long double> C;
    Mat img;
    int w_;
    int h_;
    int nbr_thread_;
    int N_ITER_;
    float color[12];

    long double THRESHOLD_ = 1000.f;
    long double zoom_;
    long double step_x_;
    long double step_y_;
};