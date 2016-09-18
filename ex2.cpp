#include <iostream>
#include <highgui.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int qtdArg, char** args) {
    Mat image;

    image= imread(args[1], CV_LOAD_IMAGE_COLOR);
    int width = image.size().width;
    int height = image.size().height;

    //Mapeando Rect
    Mat X(image, Rect(0, 0, width/2, height/2));
    Mat W(image, Rect(width/2, 0, width/2, height/2));
    Mat Y(image, Rect(0, height/2, width/2, height/2));
    Mat Z(image, Rect(width/2, height/2, width/2, height/2));

    //Cria nova Mat
    Mat saida = Mat::zeros(image.size(), image.type());
    Mat aux;

    //MApeia nova região da matriz de saída com um auxiliar 
    aux = saida.colRange(0, width/2).rowRange(0, height/2);
    //Copia o conteúdo
    Z.clone().copyTo(aux);

    aux = saida.colRange(width/2, width).rowRange(0, height/2);
    Y.copyTo(aux);

    aux = saida.colRange(0, width/2).rowRange(height/2, height);
    W.copyTo(aux);

    aux = saida.colRange(width/2, width).rowRange(height/2, height);
    X.copyTo(aux);

    if(!image.data)
        cout << "falha ao abrir a imagem biel.png" << endl;

    namedWindow("janela", WINDOW_AUTOSIZE);
    imshow("janela", saida);
    waitKey();
    return 0;

}