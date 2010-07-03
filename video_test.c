// 動画ファイルの読み込み
// p で再生、停止
// s で現在のフレームを画像として保存
// トラックバーつき
// 再生時にトラックバー名横の数字が更新されないのは謎

#include <cv.h>
#include <highgui.h>
#include <ctype.h>

int slider_position = 0;
CvCapture *capture = 0;
IplImage *frame = 0;

void usage(){
  printf("Usage: ./video_test video_file_name\n");
}

void onTrackBarSlide(int pos){
  cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, pos);
  frame = cvQueryFrame (capture);
  cvShowImage ("Capture", frame);
}

int
main (int argc, char **argv)
{
  //読み込む動画ファイル名
  char* filename;
  char* imagename;
  if (argc == 2){
    filename = argv[1];
  }else{
    usage();
    return -1;
  }

    printf ("########### #############\n"
      "video_test, using OpenCV version %s (%d.%d.%d)\n",
	    CV_VERSION,
	    CV_MAJOR_VERSION, CV_MINOR_VERSION, CV_SUBMINOR_VERSION);

    printf( "Hot keys: \n"
            "\tESC - quit the program\n"
            "\tp - play/stop\n"
            "\ts - save current frame as jpg\n"
            "\tother key - next frame\n"
            "\n" );

  double w = 320, h = 240;
  int c;

  //指定したAVIファイルが見つからない場合
  if(NULL==(capture = cvCaptureFromAVI(filename))){
    fprintf(stderr,"指定のaviファイル %s が見つかりませんでした．", filename);
    return -1;
  }

  // キャプチャサイズを設定する．
  cvSetCaptureProperty (capture, CV_CAP_PROP_FRAME_WIDTH, w);
  cvSetCaptureProperty (capture, CV_CAP_PROP_FRAME_HEIGHT, h);

  cvNamedWindow ("Capture", CV_WINDOW_AUTOSIZE);

  //スライドバーとかつけちゃう
  int frames = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT);
  if(frames != 0){
    cvCreateTrackbar("Position", "Capture", &slider_position, frames, onTrackBarSlide);
  }

  int play = 0;
  // ビデオから画像をキャプチャする
  while (1) {
    frame = cvQueryFrame (capture);

    slider_position = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES);
    cvSetTrackbarPos("Position", "Capture", slider_position);

    cvShowImage ("Capture", frame);

    if(play){
      c = cvWaitKey (2);
    }else{
      c = cvWaitKey (0);
    }

    // ESC で終了
    if (c == '\x1b')
      break;
 
    // キー入力色々 
    switch( (char) c ) {
      case 'p':
          play ^= 1;
          break;
      case 's':
          //printf("%d\n", num);
          // ↓なぜかBus error
          //sprintf(imagename, "image_%d.jpg", num);

          imagename = "image.jpg";
          cvSaveImage(imagename, frame);
          printf("Current frame is saved as %s\n", imagename); 
          break;
      default:
          ;
    }
  }

  cvReleaseCapture (&capture);
  cvDestroyWindow ("Capture");

  return 0;
}
