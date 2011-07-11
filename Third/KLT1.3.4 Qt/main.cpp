#include <fstream>
#include "klt.h"
#include <QtGui/QApplication>
#include <QImage>

using namespace std;

int main(int argc, char *argv[])
{
  unsigned char *img1, *img2;
  KLT_TrackingContext tc;
  KLT_FeatureList fl;
  int nFeatures = 100;
  int ncols, nrows;
  int i;

  tc = KLTCreateTrackingContext();
  KLTPrintTrackingContext(tc);
  fl = KLTCreateFeatureList(nFeatures);

  QImage image1("teste1.png");
  QImage image2("teste2.png");
  img1 = image1.bits();
  img2 = image2.bits();

  ncols = image1.width();
  nrows = image1.height();
  KLTSelectGoodFeatures(tc, img1, ncols, nrows, fl);

  printf("\nIn first image:\n");
  for (i = 0 ; i < fl->nFeatures ; i++)  {
    printf("Feature #%d:  (%f,%f) with value of %d\n",
           i, fl->feature[i]->x, fl->feature[i]->y,
           fl->feature[i]->val);
  }

  ofstream f1("xy.txt");
  f1 << fl->nFeatures << endl;
  for (i = 0 ; i < fl->nFeatures ; i++)  {
    f1 << fl->feature[i]->x+100 << endl;
    f1 << fl->feature[i]->y+100 << endl;
  }
  
  KLTTrackFeatures(tc, img1, img2, ncols, nrows, fl);

  printf("\nIn second image:\n");
  for (i = 0 ; i < fl->nFeatures ; i++)  {
    printf("Feature #%d:  (%f,%f) with value of %d\n",
           i, fl->feature[i]->x, fl->feature[i]->y,
           fl->feature[i]->val);
  }

 
  ofstream f2("xy_Book2.txt");
  f2 << fl->nFeatures << endl;
  for (i = 0 ; i < fl->nFeatures ; i++)  {
    f2 << fl->feature[i]->x << endl;
    f2 << fl->feature[i]->y << endl;
  }

  //KLTWriteFeatureList(fl, "feat2.txt", "%5.1f");  /* text file   */

  return 0;

}

