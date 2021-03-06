#include <QApplication.h>
#include "Ptr.h"
#include "Object.h"
#include "Drawable.h"

#include "EditorInfo/EditorGlobalInfo.h"
#include "EditorAssist/TerrainBrush.h"
#include "EditorManager/EditorRoot.h"

namespace Urho3D
{
    class Context;
    class Engine;

    class Sprite;
    class Node;
    class Scene;
	class Terrain;
}

class MainWindow;

using namespace Urho3D;

/// Editor application class.
class EditorApplication : public QApplication, public Object
{
    Q_OBJECT
    OBJECT(EditorApplication)

public:
    EditorApplication(int argc, char** argv,Context* context);
    virtual ~EditorApplication();
	
	void SetStyleSheet(const char* qssPath);

    int Run();
private slots:
    void OnTimeout();

private:
	void InitUI();

    /// �������
    void CreateDebugHud();

    //keyDown�¼�
    void HandleKeyDown(StringHash eventType, VariantMap& eventData);

    /// Read input and moves the camera.
    void MoveCamera(float timeStep);
    /// Subscribe to application-wide logic update events.
    void SubscribeToEvents();
    /// Handle the logic update event.
    void HandleUpdate(StringHash eventType, VariantMap& eventData);
private:
	void HandleLogMessage(StringHash eventType, VariantMap& eventData);

    SharedPtr<Engine> engine_;

    MainWindow* mainWindow_;
	EditorRoot* editorRoot_;
};
