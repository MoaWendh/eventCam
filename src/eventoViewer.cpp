// Moacir
// Para testar as configurações dos biases.
// Data: 17/07/2025

#include <metavision/sdk/driver/camera.h>
#include <metavision/sdk/base/events/event_cd.h>

#include <metavision/sdk/core/algorithms/periodic_frame_generation_algorithm.h>
#include <metavision/sdk/ui/utils/window.h>
#include <metavision/sdk/ui/utils/event_loop.h>

#include "eventoViewer.hpp"


// Esta função é executada ocorre a chamada da callback do botão 'Visualizar eventos':
// Esta função executa o pipeline completo da camera de eventos:
// 1- Instancia a camera;
// 2- Instancia o objeto analizsador de eventos;
// 3- chama o cam.start();
// 4- no final chama cam.stop(). Encerrando os processos e threads de forma segura.
// OU seja, ao final ela fecha e destroi o objeto criado, se quiser ver os eventos em outra função, 
// o objeto deverá ser criado novamente 
void frameEventView(int argc, char *argv[]){
    // Instancia uma câmera de eventos chama "cam":
    Metavision::Camera cam; 

    // Instancio um objeto event_analyzer
    EventAnalyzer event_analyzer; // create the event analyzer
    
    // Dependendo do argumento passado em vez de abrir a câmera, será aberto um arquivo pré-gravado
    // ou seja, um arquivo de eventos anteriormente registrados:
    // A chamad do program sem argumentos abrirá o device da câmera
    if (argc >= 2 && argv[1] != nullptr)  {
        // if we passed a file path, open it
        // Passanod o arquivo bruto "monitoring_40_50hz.raw" ao chamar o executável "eventCam":
        // por exempĺo: ./eventCam monitoring_40_50hz.raw
        cam = Metavision::Camera::from_file(argv[1]);
    } else {
        // open the first available camera
        // Abre a primeira câmera disponível na USB:
        cam = Metavision::Camera::from_first_available();
    }

    // INicializa a cãmera "cam" que foi instanciada: 
    cam.start();

    // Para visualizar os eventos, é necessário construir quadros (frames virtuais) e renderizá-los.
    // A construção dos quadros será realizada por um gerador de quadros que acumulará os eventos ao longo do tempo.
    // Parta isso, é necessário fornecer a resolução da câmera, que pode ser obtida a partir da instância da câmera
    int camera_width  = cam.geometry().width();
    int camera_height = cam.geometry().height();

    // Também é necessário efinir o tempo acumulado, duração que define o volume de eventos, e ambém um frame rate.
    // Valores pré=definidos:  20ms e 50fps:
    const std::uint32_t acc = 20000;
    double fps              = 50;

    // Usa-se a função "Metavision::PeriodicFrameGenerationAlgorithm() para informar, configurar, o gerador de eventos
    // dos parâmetros pré-definidos:
      auto frame_gen = Metavision::PeriodicFrameGenerationAlgorithm(camera_width, camera_height, acc, fps);

    // Define-se a callback que irá ser chamada para passar os eventos ao gerador de frames virtuais:  
    cam.cd().add_callback([&](const Metavision::EventCD *begin, const Metavision::EventCD *end) {
        frame_gen.process_events(begin, end);
    });

    // Para renderizar os frames virtuais, cria-se uma janela da classe "Window" pertencente ao módulo "UI":
    Metavision::Window window("Metavision SDK Get Started", camera_width, camera_height,
                              Metavision::BaseWindow::RenderMode::BGR);

    // Configura-se a callback para o fechamaento da janela quando as teclas "esc ou "Q" forem pressionadas:
    window.set_keyboard_callback(
        [&window](Metavision::UIKeyEvent key, int scancode, Metavision::UIAction action, int mods) {
            if (action == Metavision::UIAction::RELEASE &&
                (key == Metavision::UIKeyEvent::KEY_ESCAPE || key == Metavision::UIKeyEvent::KEY_Q)) {
                window.set_close_flag();
            }
        });

    // Define-se a callback no gerador de frams virtuais para chamar o objeto Window e exibir os frames:
    frame_gen.set_output_callback([&](Metavision::timestamp, cv::Mat &frame) { 
        window.show(frame); 
    });

    // keep running until the camera is off, the recording is finished or the escape key was pressed
    while (cam.is_running() && !window.should_close()) {
        // we poll events (keyboard, mouse etc.) from the system with a 20ms sleep to avoid using 100% of a CPU's core
        // and we push them into the window where the callback on the escape key will ask the windows to close
        static constexpr std::int64_t kSleepPeriodMs = 20;
        Metavision::EventLoop::poll_and_dispatch(kSleepPeriodMs);
    }
    // Chama a função cam.stop para finalizar a cãmera:
    cam.stop();
}




// Esta função pertence a classe "EventAnalyzer", conforme declarad m "funcoesEventos.hpp".
// Esta função será executada na chamada da callback da câmera de eventos semrep que um evento "CD" for disponibilizado:
// Ela será chamada sempre que novos eventos do tipo CD (Change Detection) estão disponíveis. 
// Para ser compatível com a callback, uma função precisa retornar void e ter dois parâmetros de entrqda. 
// Um para o "início" e outro para o "fim" do buffer de eventos que serão disponibilizados para a função. 
// Esses dois iteradores nos permitem acessar todos os novos eventos passados por meio dessa callback. 
// Essa função é equivalete a função definida abaixo (fora da classe) "count_events(const Metavision::EventCD *begin, const Metavision::EventCD *end)".
// É mais prático usar a função definida dentro da classe.
void EventAnalyzer::analyze_events(const Metavision::EventCD *begin, const Metavision::EventCD *end) {
    // time analysis
    // Note: events are ordered by timestamp in the callback, so the first event will have the lowest timestamp and
    // the last event will have the highest timestamp
    Metavision::timestamp min_t = begin->t;     // get the timestamp of the first event of this callback
    Metavision::timestamp max_t = (end - 1)->t; // get the timestamp of the last event of this callback
    global_max_t = max_t; // events are ordered by timestamp, so the current last event has the highest timestamp

    // O laço for permite acessar todos os eventos capturado nessa callback. 
    int counter = 0;
    for (const Metavision::EventCD *ev = begin; ev != end; ++ev) {
        ++counter; // Incrementa o contador de eventos
    }

    // Incrementa o contador global:
    global_counter += counter; 

    // Retirar o comentário das linnhas abaixo para exibir o relatório do buffer no terminal.
    // Atenção!!! Registrar informações no terminal pode reduzir drasticamente o desempenho da sua aplicação, 
    // especialmente em plataformas embarcadas com baixo poder computacional.
    // std::cout << "Cb n°" << callback_counter << ": " << counter << " events from t=" << min_t << " to t="
    // << max_t << " us." << std::endl;

    //Incrementa o contador de chamadas da callback:
    callback_counter++;
}



// Esta função será executada na chamada da callback da câmera de eventos semrep que um evento CD for disponibilizado:
// Ela será chamada sempre que novos eventos do tipo CD (Change Detection) estão disponíveis. 
// Para ser compatível com a callback, uma função precisa retornar void e ter dois parâmetros de entrqda. 
// Um para o "início" e outro para o "fim" do buffer de eventos que serão disponibilizados para a função. 
// Esses dois iteradores nos permitem acessar todos os novos eventos passados por meio dessa callback.  
void count_events(const Metavision::EventCD *begin, const Metavision::EventCD *end) {
    int counter = 0;

    // O laço for permite acessar todos os eventos capturado nessa callback. 
    for (const Metavision::EventCD *ev = begin; ev != end; ++ev) {
        ++counter; // Incrementa o contador de eventos

        // Exibe os valores do buffer de eventos: coordenadas, timestamp e a polaridade:
        std::cout << "Evento recebido: coordenadas (" << ev->x << ", " << ev->y << "), time-stamp: " << ev->t
                  << ", polaridade: " << ev->p << std::endl;
    }

    // report
    std::cout << "Ocorreram " << counter << " eventos nessa callback!!" << std::endl;
}


