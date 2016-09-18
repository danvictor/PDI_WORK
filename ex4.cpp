#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat image, eqImg;
  int width, height;
  VideoCapture cap;
  vector<Mat> planes, planesEq;
  Mat histR, histG, histB;
  int nbins = 64;
  float range[] = {0, 256};
  const float *histrange = { range };
  bool uniform = true;
  bool acummulate = false;

  cap.open(0);

  if(!cap.isOpened()){
    cout << "cameras indisponiveis";
    return -1;
  }

  width  = cap.get(CV_CAP_PROP_FRAME_WIDTH);
  height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

  //cap.set(CV_CAP_PROP_POS_FRAMES,100);

  cout << "largura = " << width << endl;
  cout << "altura  = " << height << endl;

  int histw = nbins, histh = nbins/2;
  Mat histImgR(histh, histw, CV_8UC3, Scalar(0,0,0));
  Mat histImgG(histh, histw, CV_8UC3, Scalar(0,0,0));
  Mat histImgB(histh, histw, CV_8UC3, Scalar(0,0,0));

  while(1){
    cap >> image;
	flip(image, image, 1);

	//--etapa 1-- cria nova imagem(eqimg), equaliza e exibe
    cvtColor(image, eqImg, CV_BGR2YCrCb); //muda o formato de cor da imagem para YCrCb
    split(eqImg,planesEq);
    equalizeHist(planesEq[0], planesEq[0]); //equaliza histograma para o primeiro canal Y
    merge(planesEq, eqImg); //junta os planos incluindo o plano equalizado(intensidade de luz)
    cvtColor(eqImg, eqImg, CV_YCrCb2BGR); //modifica o formato de cores para RGB novamente para exibição
    //--FIM etapa1--

    split (image, planes);
    calcHist(&planes[0], 1, 0, Mat(), histR, 1,
             &nbins, &histrange,
             uniform, acummulate);


    normalize(histR, histR, 0, histImgR.rows, NORM_MINMAX, -1, Mat());
    histImgR.setTo(Scalar(0));


    for(int i=0; i<nbins; i++){
      line(histImgR, Point(i, histh),
           Point(i, cvRound(histR.at<float>(i))),
           Scalar(0, 0, 255), 1, 8, 0);

    }
    histImgR.copyTo(image(Rect(0, 0       ,nbins, histh)));


	namedWindow("Original Image", CV_WINDOW_AUTOSIZE);
	namedWindow("Equalized Image", CV_WINDOW_AUTOSIZE);

    imshow("Original Image", image);
	imshow("Equalized Image", eqImg);

    if(waitKey(30) >= 0) break;
  }
  return 0;
}