
#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <iostream>

#include <metavision/sdk/driver/camera.h>

#include "menuGUI.hpp"
#include "eventoViewer.hpp" 


void launchMenuGUI(int argc, char *argv[]) {
    int argc_Qt = 0;
    char **argv_Qt = nullptr;
    QApplication app(argc_Qt, argv_Qt);

    // Instancia a janela principal
    QWidget window;
    window.setWindowTitle("Menu Principal");
    window.resize(300, 150);

    // Cria os botões
    QPushButton *btnVisualizar = new QPushButton("Visualizar eventos", &window);
    QPushButton *btnSair = new QPushButton("Sair", &window);

    // Layout vertical
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(btnVisualizar);
    layout->addWidget(btnSair);
    window.setLayout(layout);

    // Conecta botão "Visualizar eventos"
    QObject::connect(btnVisualizar, &QPushButton::clicked, [&]() {
        std::cout << "Botão 'Visualizar eventos' pressionado." << std::endl;
        frameEventView(argc, argv); 
    });

    // Conecta botão "Sair"
    QObject::connect(btnSair, &QPushButton::clicked, [&]() {
        std::cout << "Botão 'Sair' pressionado."<< std::endl << "Encerrando aplicação..." << std::endl;
        app.quit();
    });

    // Essa chamada "window.show()" torna a janela visível, sem ela a janela é instanciada, mas não aparece para o usuário.
    window.show();

    // Essa chamada "app.exec()" inicia o loop de eventos da aplicação Qt — também conhecido como event loop.
    // Ela faz a interface ficar rodando continuamente até ser solicitado um eveto por click do mouse:
    app.exec();
}

