
#include <vector>
#include "App.h"
using namespace std;


int main() {
    Funccia::App& app = Funccia::App::Instance();
    app.Init();
    app.Update();
    app.Shutdown();
    return 0;
}