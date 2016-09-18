#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv){
  Mat image, mask;
  int width, height;
  int C_Bolha = 67;
  int S_Bolha = 196;
  CvPoint p;
  p.x = 0;
  p.y = 0;
  image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);

  if(!image.data){
    std::cout << "Falha ao abrir imagem\n";
    return(-1);
  }
  imshow("imageOriginal", image);
  width = image.size().width;
  height = image.size().height;

  //Pintar de branco os elementos da borda 
  for (int i = 0; i < width; i++) {
    image.at<uchar>(i, height -1) = 255;
    image.at<uchar>(i, 0) = 255;
  }

  for (int i = 0; i < height; i++) {
    image.at<uchar>(0, i) = 255;
    image.at<uchar>(width -1, i) = 255;
  }
//Elimina todos elementos que tocam as bordas
  floodFill(image, p, 0);

//Definicao do "fundo da imagem"
  floodFill(image, p, 1);


  //Procurando todos elementos
  int qtdTotal = 0;
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if (image.at<uchar>(i, j) == 255) {
        p.x = j;
        p.y = i;
        floodFill(image, p, S_Bolha);
        qtdTotal++;
      }
    }
  }

  //Procurando apenas elementos que possuam bolhas
  int qtdComBolhas = 0;
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      //Armazena a posição do elemento encontrado, independente se for rotulado com bolha ou sem bolha
      if (image.at<uchar>(i,j) == S_Bolha || image.at<uchar>(i,j) == C_Bolha) {
        p.x = j;
        p.y = i;
      } else if (image.at<uchar>(i,j) == 0) {
        //Caso for encontrado um buraco, verficar se ja foi rotulado, caso contrario rotule o ultimo elemento encontrado como "C_Bolha"
        if (image.at<uchar>(p.y, p.x) == S_Bolha) {
          floodFill(image, p, C_Bolha);
          qtdComBolhas++;
        }
        //Rotule o buraco encontrado como C_Bolha
        p.x = j;
        p.y = i;
        floodFill(image, p, C_Bolha);
      }
    }
  }

  imshow("image", image);
  imwrite("SoSolidos", image);
  printf("elementos sem bolhas: %d, com bolhas: %d\n", qtdTotal - qtdComBolhas, qtdComBolhas);
  waitKey();
  return 0;
}