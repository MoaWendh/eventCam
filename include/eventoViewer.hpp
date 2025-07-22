#ifndef FUNCOES_EVENTOS_HPP
#define FUNCOES_EVENTOS_HPP

#include <metavision/sdk/driver/camera.h>
#include <metavision/sdk/base/events/event_cd.h>



// Classe dedicada para processar eventos CD (Change Detection)
class EventAnalyzer {
public:
    // Variáveis de controle e análise
    int callback_counter = 0;
    int global_counter = 0;
    Metavision::timestamp global_max_t = 0;

    // Função chamada como callback para processar os eventos
    void analyze_events(const Metavision::EventCD *begin, const Metavision::EventCD *end);
};


void count_events(const Metavision::EventCD *begin, const Metavision::EventCD *end);
void frameEventView(int argc, char *argv[]);

#endif // FUNCOES_EVENTOS_HPP