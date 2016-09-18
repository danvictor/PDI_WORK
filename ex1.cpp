#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int, char**){
  Mat image;

  int width, height;
  Vec4i pontos; //vetor para os pontos P1 e P2

  int Cpixel;

  image= imread("biel.png",CV_LOAD_IMAGE_GRAYSCALE);
  if(!image.data)
    cout << "nao abriu biel.png" << endl;

//pega o tamanho da imagem, em pixels
  width=image.size().width;
  height=image.size().height;

  namedWindow("janela",WINDOW_AUTOSIZE);

  cout << "Escolha dois pontos (x,y) da imagem, levando em consideração que a imagem é de tamanho " << width << "x" << height << "." << endl;

  cout << "Ponto P1:" << endl << "x = ";
  cin >> pontos[0];
  cout << "y = ";
  cin >> pontos[1];

  cout << "Ponto P2:" << endl << "x = ";
  cin >> pontos[2];
  cout << "y = ";
  cin >> pontos[3];

//condições de "tamanho" das coordenadas, para que o loops funcionem

  if((pontos[2]>pontos[0])&(pontos[3]>pontos[1])){
	  for(int i=pontos[0];i<pontos[2];i++){
	    for(int j=pontos[1];j<pontos[3];j++){
	      Cpixel = image.at<uchar>(i,j); //pega a cor atual do pixel em (i,j)
	      image.at<uchar>(i,j)= 255 - Cpixel; //negativo
	    }
	  }
  } else if((pontos[0]>pontos[2])&(pontos[3]>pontos[1])){
	  for(int i=pontos[2];i<pontos[0];i++){
	    for(int j=pontos[1];j<pontos[3];j++){
	      Cpixel = image.at<uchar>(i,j);
	      image.at<uchar>(i,j)= 255 - Cpixel;
	    }
	  }
  } else if((pontos[2]>pontos[0])&(pontos[1]>pontos[3])){
	  for(int i=pontos[0];i<pontos[2];i++){
	    for(int j=pontos[3];j<pontos[1];j++){
	      Cpixel = image.at<uchar>(i,j);
	      image.at<uchar>(i,j)= 255 - Cpixel;
	    }
	  }
  } else {
  	for(int i=pontos[2];i<pontos[0];i++){
	    for(int j=pontos[3];j<pontos[1];j++){
	      Cpixel = image.at<uchar>(i,j);
	      image.at<uchar>(i,j)= 255 - Cpixel;
	    }
	  }
  }
  imshow("janela", image);
  imwrite("negativo.png", image);
  waitKey();

  return 0;
}