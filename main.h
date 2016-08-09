#include <Inventor/SoDB.h>
#include <Inventor/SoSceneManager.h>

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoPointLight.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>

#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoShapeHints.h>

#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoScale.h>

#include <Inventor/nodes/SoSelection.h>
#include <Inventor/nodes/SoEventCallback.h>

#include <Inventor/events/SoEvent.h>
#include <Inventor/events/SoEvents.h>

#include <Inventor/sensors/SoTimerSensor.h>

#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/SoXtRenderArea.h>

#include <math.h>
#define PI 3.1415
#define DEBUG(x) puts(x)

