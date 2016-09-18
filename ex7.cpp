#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int height;

int MAX = 100;
double decaimento = 6;
int decaimento_slider = 0;

int top_slider = 0;
int vertical = 0;

int faixa = 20;
int altura_slider = 0;


Mat image1, image2, alpha, beta;

char TrackbarName[50];

void Calc(){
  Mat ponderada1, ponderada2, tiltshift;
  int l1 = - faixa/2;
    int l2 = -l1;
    alpha = Mat::zeros(image1.rows, image1.cols, CV_32F);
    beta = Mat::zeros(image1.rows, image1.cols, CV_32F);
    int i, j;
    for (i = 0; i < alpha.rows; i++) {
        int x = i - (vertical + faixa/2);
        float alphaValue = 0.5f * (tanh((x - l1)/decaimento) - tanh((x - l2)/decaimento));
        for (j = 0; j < alpha.cols; j++) {
            alpha.at<float>(i, j) = alphaValue;
            beta.at<float>(i, j) = 1 - alphaValue;
        }
    }
    Mat auxA[] = {alpha, alpha, alpha};
    Mat auxB[] = {beta, beta, beta};
    merge(auxA, 3, alpha);
    merge(auxB, 3, beta);

  image1.convertTo(ponderada1, CV_32FC3);
    image2.convertTo(ponderada2, CV_32FC3);

    multiply(ponderada1, alpha, ponderada1);
    multiply(ponderada2, beta, ponderada2);

    add(ponderada1, ponderada2, tiltshift);
    tiltshift.convertTo(tiltshift, CV_8UC3);
    imshow("tiltshift", tiltshift);

    imshow("imagem", alpha);
  imshow("borramento", beta);
}

void on_trackbar_decaimento(int, void*){
    decaimento = (double) decaimento_slider;
    if (decaimento < 1) {
        decaimento = 1;
    }
    Calc();
}

void on_trackbar_vertical(int, void *){
    vertical = top_slider*height/MAX;
    Calc();
}

void on_trackbar_altura(int, void*) {
    faixa = altura_slider*height/MAX;
    if (faixa == 0) {
        faixa = 1;
    }

    if (faixa > height) {
        faixa = height;
    }
    Calc();
}

int main(int argvc, char** argv){
  image1 = imread("cidade.png");
  height = image1.size().height;
  image2 = image1.clone();

  Mat aux, mask, mask1;
    float media[] = {1,1,1,
                     1,1,1,
                     1,1,1};
    //para efeito de borramento
    mask = Mat(3, 3, CV_32F, media);
    scaleAdd(mask, 1/9.0, Mat::zeros(3,3,CV_32F), mask1);
    mask = mask1;
    image2.convertTo(aux, CV_32F);
    for (int i = 0; i < 10; i++) {
        filter2D(aux, aux, aux.depth(), mask, Point(1, 1), 0);
    }
    aux=abs(aux);
    aux.convertTo(image2, CV_8UC3);

    namedWindow("tiltshift", 1);

    sprintf( TrackbarName, "Altura da região");
    createTrackbar( TrackbarName, "tiltshift",
                    &altura_slider,
                    MAX,
                    on_trackbar_altura);

    sprintf( TrackbarName, "Decaimento");
    createTrackbar( TrackbarName, "tiltshift",
                    &decaimento_slider,
                    MAX,
                    on_trackbar_decaimento);

    sprintf( TrackbarName, "Posição Vertical");
    createTrackbar( TrackbarName, "tiltshift",
                    &top_slider,
                    MAX,
                    on_trackbar_vertical );
    Calc();

  waitKey(0);
  return 0;
}