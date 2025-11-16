#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QTableWidgetItem;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // === NAVIGATION ===
  void on_gestionhabitat_button_clicked();
    void on_surveilalnce_button_clicked();

    // === HABITAT ===
    void on_btnAjouter_clicked();
    void on_btnSupprimer_clicked();
    void on_btnModifier_clicked();
    void on_btnafficher_clicked();
    void on_TabCrudHabitat_itemClicked(QTableWidgetItem *item);

    // === CONSOMMATION ===
    void on_AJOUTconso_clicked();
    void on_btnSupprimerConso_clicked();
    void on_btnModifierConso_clicked();
    void on_table_surveillance_itemClicked(QTableWidgetItem *item);

    // === TRI ET RECHERCHE ===
    void on_comboTriConso_currentIndexChanged(int index);
    void on_lineRechercheHabitat_textChanged(const QString &text);
    void on_lineRechercheEtat_textChanged(const QString &text);
    void on_btnReinitialiserFiltre_clicked();


    // === STATISTIQUES ===
    void on_btnStatistiques_clicked();

private:
    Ui::MainWindow *ui;
    QString critereTri;

    void initialiserTable();
    void clearFieldsHabitat();
    void clearFieldsConso();
    void afficherConsommations();
    void exporterStatistiquesPDF();
    void exporterStatistiquesCSV();
    QString genererHTMLStatistiques();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // MAINWINDOW_H
