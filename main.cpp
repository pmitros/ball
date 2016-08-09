/* By Piotr Mitros. 
Copyright (c) 2002. All Rights Reserved. 
May be distributed under GPL. 
*/
#include "main.h"

struct Ball {
  // Constructor will be necessary when new's/malloc'd
  Ball() {
    position=0; light=0; x=y=z=dx=dy=dz=0; 
    up=down=left=right=ix=iy=0;
  }
  SoTranslation *position;
  SoPointLight *light;
  SoPerspectiveCamera *camera;
  float x, y, z;
  float dx, dy, dz;
  int ix, iy;
  int up, down, left, right;
};

SoSeparator* loadModel(const char* fileName);
SoSeparator* createScene(void) ;
void keyboardCB(void*, SoEventCallback*);
void loadLevel(SoSeparator*map);

Ball player;

int mapX, mapY;
char **map;

int mapAt(int x, int y)
{
  if((x<0)||(y<0)||(x>=mapX))
    return 0;
  if(y>=mapY)
    return 1;
  return map[y][x];
}

void loadLevel(SoSeparator*root)
{
  FILE*f=fopen("level.dat", "r");
  if(!f) {
    printf("Couldn't open menu\n");
    exit(0);
  }
  fscanf(f, "%i %i\n", &mapX, &mapY);
  DEBUG("Loaded map");
  map=new (char*)[mapY];
  for(int i=0; i<mapY; i++) {
    map[i]=new char[mapX];
    for(int j=0; j<mapX; j++) {
      map[i][j]=fgetc(f)-'0';
      if(map[i][j]) {
	SoTranslation*position=new SoTranslation;
	SoCube*cube=new SoCube;
	SoSeparator*separator=new SoSeparator;
	SoScale *scale=new SoScale;
	scale->scaleFactor.setValue(SbVec3f(1, 0.5, 1));
	position->translation.setValue(SbVec3f((j-mapX/2)*2, map[i][j], -i*2));
	separator->addChild(scale);
	separator->addChild(position);
	separator->addChild(cube);
	root->addChild(separator);
      }
    }
    fgetc(f);
  }
  DEBUG("Done reading\n");
  fclose(f);
}

// the MAIN function
int main(int argc, char ** argv)
{
  Widget mainWindow = SoXt::init(argv[0]);
  if (mainWindow == NULL) exit(1);

  SoSeparator *root;

  root = new SoSeparator;
  root->ref();

  player.camera = new SoPerspectiveCamera;
  root->addChild(player.camera);
  root->addChild(createScene()) ;

  //player.camera->viewAll(root, myRenderArea->getViewportRegion());
  player.camera->position=SbVec3f(0, 5, 20);
  player.camera->orientation=SbRotation(-0.1, 0, 0, 1);
  player.camera->nearDistance=1;
  player.camera->farDistance=150;
  player.camera->heightAngle=0.5;

  //view the whole scene

  // initialize scenemanager instance
  SoXtRenderArea *myRenderArea = new SoXtRenderArea(mainWindow);
  myRenderArea->setSceneGraph(root);
  myRenderArea->setTitle("Bouncing Ball");
  myRenderArea->show();
  
  SoXt::show(mainWindow);
  SoXt::mainLoop();
  
  return 0;
}

void increment(void *, SoSensor *)
{
  player.x+=player.dx;
  player.y+=player.dy;
  player.z+=player.dz;

  player.ix=int(player.x+101)/2+mapX/2-50;
  player.iy=int(-player.y+101)/2-50;

  player.dz-=0.1;
  if(player.z<0) {
    //    if((player.ix>=0)&&(player.iy>=0)&&(map[player.iy][player.ix])) {
    if((player.ix>=0)&&(player.iy>=0)&&(mapAt(player.ix,player.iy))) {
      player.dz=0;
      player.z=0;
    }
    else {
      player.x=player.dz=player.dy=player.dx=player.y=player.z=0;
    }
  }
  player.position->translation.setValue(SbVec3f(0+player.x,
						1.5+player.z,
						0+player.y));
  player.light->location.setValue(SbVec3f(0+player.x,5+player.z,0+player.y));
  player.camera->position=SbVec3f(0+player.x, 5+player.z, 20+player.y);
  //printf("%f %f %i %i\n",player.x, player.y,player.ix, player.iy);

  // Update acceleration based on whether player is holding arrow key down
  // Math used to set maximum speed of 2, and higher acceleration when away
  // from max speed. 
  // Also, if speed is near zero, and player is trying to stop, ball stops, 
  // rather than taking on pathetically slow velocities. 
  if(player.left)
    if((player.dx<=0)||(player.dx>.2))
      player.dx-=.1*(2+player.dx);
    else
      player.dx=0;
  if(player.right)
    if((player.dx>=0)||(player.dx<-.2))
      player.dx+=.1*(2-player.dx);
    else
      player.dx=0;

  if((!player.left)&&(!player.right)) {
    if(fabs(player.dx<0.01))
      player.dx=0;
    else
      player.dx*=0.8;
  }

  if(player.up)
    player.dy-=.1*(2+player.dy);
  if(player.down)
    player.dy+=.1*(2-player.dy);
}

SoSeparator* createScene(void)
{
  SoSeparator *root = new SoSeparator ;

  player.light=new SoPointLight;
  player.light->location.setValue(SbVec3f(0,5,0));
  player.light->intensity.setValue(1);
  root->addChild(player.light);

  SoDirectionalLight*dl1=new SoDirectionalLight;
  // -1 right, 1 left : -1 top, 1 bottom : -1 back, 1 front
  dl1->direction.setValue(SbVec3f(0, -1, -0.5));
  dl1->intensity.setValue(1);
  root->addChild(dl1);

  SoMaterial *mat2 = new SoMaterial ;
  mat2->diffuseColor.setValue(0.0,0.7,0.7) ;
  mat2->ambientColor.setValue(0.0,0.0,0.7) ;
  mat2->specularColor.setValue(0.0,1.0,0.0) ;
  root->addChild(mat2) ;
  SoDrawStyle * drawstyle2 = new SoDrawStyle;
  drawstyle2->style.setValue(SoDrawStyleElement::FILLED);

  root->addChild(drawstyle2);
  SoSeparator *ballSep=new SoSeparator;
  SoSphere *ball=new SoSphere;
  player.position=new SoTranslation;
  player.position->translation.setValue(SbVec3f(0,1.5,0));
  ball->radius.setValue(0.5);
  ballSep->addChild(player.position);
  ballSep->addChild(ball);
  root->addChild(ballSep);

  //FILLED, LINES, POINTS, INVISIBLE 
  SoDrawStyle * drawstyle = new SoDrawStyle;
  drawstyle->style.setValue(SoDrawStyleElement::FILLED);
  //drawstyle->style.setValue(SoDrawStyleElement::LINES);
  root->addChild(drawstyle);
  
  SoMaterial *mat = new SoMaterial ;
  mat->diffuseColor.setValue(0.7,0.7,0.0) ;
  mat->ambientColor.setValue(0.7,0.0,0.0) ;
  mat->specularColor.setValue(0.0,1.0,0.0) ;
  mat->shininess.setValue(1.0) ;
  root->addChild(mat) ;

  loadLevel(root);

  SoTimerSensor *ts=new SoTimerSensor(increment, (void*)0);
  ts->schedule();

  SoEventCallback*cb=new SoEventCallback;
  cb->addEventCallback(SoKeyboardEvent::getClassTypeId(), keyboardCB, NULL);
  root->addChild(cb);

  return root ;
}

void keyboardCB(void*, SoEventCallback*cb)
{
  //printf("Keyboard\n");
  if(SO_KEY_PRESS_EVENT(cb->getEvent(), Q))
    exit(0);
  if(SO_KEY_PRESS_EVENT(cb->getEvent(), LEFT_ARROW))
    player.left=1;
  if(SO_KEY_PRESS_EVENT(cb->getEvent(), RIGHT_ARROW))
    player.right=1;
  if(SO_KEY_PRESS_EVENT(cb->getEvent(), UP_ARROW))
    player.up=1;
  if(SO_KEY_PRESS_EVENT(cb->getEvent(), DOWN_ARROW))
    player.down=1;
  if(SO_KEY_RELEASE_EVENT(cb->getEvent(), LEFT_ARROW))
    player.left=0;
  if(SO_KEY_RELEASE_EVENT(cb->getEvent(), RIGHT_ARROW))
    player.right=0;
  if(SO_KEY_RELEASE_EVENT(cb->getEvent(), UP_ARROW))
    player.up=0;
  if(SO_KEY_RELEASE_EVENT(cb->getEvent(), DOWN_ARROW))
    player.down=0;

  if(SO_KEY_PRESS_EVENT(cb->getEvent(), SPACE))
    if((player.z==0)&&(player.dz<.5))
      player.dz+=0.8;

  cb->setHandled();
}

SoSeparator* loadModel(const char* fileName)
{
  SoSeparator *root = new SoSeparator;
  SoInput myScene;

  //try to open the file
  if (!myScene.openFile(fileName)) {
    printf("Could not open %s\n",fileName) ;
    return NULL;
  } 
        
  //check if the file is valid
  if (!myScene.isValidFile()) {
    printf("%s is not a valid Inventor file\n",fileName) ;
    return NULL;
  }

  //try to read the file
  root = SoDB::readAll(&myScene) ;
  
  if (root == NULL) {
    printf("Problem reading %s\n",fileName) ;
    myScene.closeFile() ;
    return NULL;
  }

  //close the file
  myScene.closeFile() ;
  
  return root ;
}
