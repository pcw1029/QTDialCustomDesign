#ifndef CUSTOMDIAL_H
#define CUSTOMDIAL_H

#include <QDial>
#include <QLabel>
#include <QLineEdit>

#define SCALE_WIDTH     200
#define TICK_LENGTH     6 // 눈금 길이
#define TEXT_AREA       29

class CustomDial : public QDial {
    Q_OBJECT
public:
    explicit CustomDial(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void onLineEditValueChanged();

private:
    QLabel *lbName;
    QLabel *lbAngleValue;
    QLineEdit *leSetAngle;

    void updateWidgetPositions();
};

#endif // CUSTOMDIAL_H


