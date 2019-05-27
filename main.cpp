#include <fractal.hpp>
#include <iostream>
int main(int argc, char const* argv[]) {
	int w, h, nbr_thread, nbr_iter;
	long double zoom, step_x, step_y, x, y;
	
	w = 800;
	h = 512;

	nbr_thread = 8;
	nbr_iter = 300;

	zoom = 0.004;
	//===================================	
	//julia:
    //===================================
    /*y = -0.78;
    x = 0.136;
    step_x = -0.002805021511944;
	step_y =  0.003064073756576;*/
	/*step_x = -0.17652855555f;
    step_y = 0.24886;*/
    /*step_x = 0.5241475999355316162f;
    step_y = 0.07860195f;*/
    /*step_x = -0.06947989f;
	step_y = -0.74910617f;*/
    //===================================
    x = 0.43f;
    y = 0.32f;
    step_x = -0.05100951f;
    step_y = -0.5200008f;
    //-----------------------------------
   /* y = 0.3336913831353;
	x = 0.39768037241511;
	step_x = -0.098316067658884;
	step_y = -0.49501652965969;
	//===================================
	//mandlbrot
	//===================================
	/*step_x = -1.7685736563152709932817429153295447129341200534055498823375111352827765533646353820119779335363321986478087958745766432300344486098206084588445291690832853792608335811319613234806674959498380432536269122404488847453646628324959064543;
	step_y = -0.0009642968513582800001762427203738194482747761226565635652857831533070475543666558930286153827950716700828887932578932976924523447497708248894734256480183898683164582055541842171815899305250842692638349057118793296768325124255746563;	//===================================*/


	Fractal frac(w, h, nbr_thread, nbr_iter, zoom, step_x, step_y, x, y);    
    
    frac.zoom_fractal_resize(1);

    /*frac.set_zoom(0.007);
    frac.set_nbr_iter(32);
    frac.fractal_parade();*/

 	//rac.show();
    return 0;
}