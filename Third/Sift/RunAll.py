import os
step=1
#folder='ImgFinalReview'
#number=205
folder='ImgDeltaFunctionDay'
number=159

for i in range(0, number, step):
   os.system(".\Release\match.exe "+folder+"\corresp"+str(i)+".png "+folder+"\corresp"+str(i+step)+".png Release\corresp"+str(i)+".txt " \
              "Release\corresp"+str(i)+"FEAT1.png Release\corresp"+str(i)+"FEAT2.png Release\corresp"+str(i)+"FEAT3.png")
#    os.system(".\Release\match.exe folder\corresp400.png ImgRecitation13\corresp"+str(i)+".png Release\corresp"+str(i)+".txt " \
#              "Release\corresp"+str(i)+"FEAT1.png Release\corresp"+str(i)+"FEAT2.png Release\corresp"+str(i)+"FEAT3.png")
