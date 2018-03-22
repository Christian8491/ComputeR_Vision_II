# DO NOT DELETE THIS LINE -- make depend depends on it.
#-lGL -lglut -lGLU -llapack, MY_CUSTOM_INCLUDE_PATH=/$(HOME)/local/include  MY_CUSTOM_LINK_PATH=/$(HOME)/local/lib

# Edit the lines below to point to any needed include and link paths
# Or to change the compiler's optimization flags
CC = g++ -std=c++11
COMPILEFLAGS = -I /$(HOME)/local/include -D_LINUX -D_REENTRANT -Wall  -O3 -march=nocona -msse3
LINKFLAGS = -L /$(HOME)/local/lib -lGVars3 -lcvd -lGL -lglut -lGLU -lm -llapack

# Edit this line to change video source
#VIDEOSOURCE = VideoSource_Linux_DV.o
VIDEOSOURCE = VideoSource_Linux_V4L.cc

OBJECTS=	main.o\
		GLWindow2.o\
		GLWindowMenu.o\
		$(VIDEOSOURCE)\
		System.o \
		ATANCamera.o\
		KeyFrame.o\
		MapPoint.o\
		Map.o\
		SmallBlurryImage.o\
		ShiTomasi.o \
		HomographyInit.o \
		MapMaker.o \
		Bundle.o \
		PatchFinder.o\
		Relocaliser.o\
		MiniPatch.o\
		MapViewer.o\
		ARDriver.o\
		ucsp.o\
		EyeGame.o\
		Tracker.o

CALIB_OBJECTS=	GLWindow2.o\
		GLWindowMenu.o\
		$(VIDEOSOURCE)\
		CalibImage.o \
		CalibCornerPatch.o\
		ATANCamera.o \
		CameraCalibrator.o

All: PTAM CameraCalibrator

PTAM: $(OBJECTS)
	$(CC) -o PTAM $(OBJECTS) $(LINKFLAGS)

CameraCalibrator:$(CALIB_OBJECTS)
	$(CC) -o CameraCalibrator $(CALIB_OBJECTS) $(LINKFLAGS)


%.o: %.cc
	$(CC) $< -o $@ -c $(COMPILEFLAGS)

clean:
	rm *.o


depend:
	rm dependecies; touch dependencies
	makedepend -fdependencies $(INCLUDEFLAGS) $(MOREINCS) *.cc *.h


-include dependencies