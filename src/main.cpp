// Moacir
// Para testar as configurações dos biases.
// Data: 17/07/2025


#include <metavision/sdk/driver/camera.h>
#include <metavision/sdk/base/events/event_cd.h>
#include <metavision/sdk/core/algorithms/periodic_frame_generation_algorithm.h>
#include <metavision/sdk/ui/utils/window.h>
#include <metavision/sdk/ui/utils/event_loop.h>

#include "eventoViewer.hpp"
#include "menuGUI.hpp"


int main(int argc, char *argv[]) {
    // Instancia uma câmera de eventos chama "cam":
    //Metavision::Camera cam; 

    // Instancio um objeto event_analyzer
    // EventAnalyzer event_analyzer; // create the event analyzer
    
    // Chama a interface GUI para interação com o usuário.
    launchMenuGUI(argc, argv);

    // INicializa a cãmera "cam" que foi instanciada: 
    //cam.start();

    // Chama a função "frameEventView()"" que irá exibir os frames virtuais da cãmera de eventos:
    // frameEventView(cam);

    // Chama a função cam.stop para finalizar a cãmera:
    //cam.stop();
}