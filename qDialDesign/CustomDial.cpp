#include "CustomDial.h"
//#include <QtMath>

//CustomDial::CustomDial(QWidget *parent)
//    : QDial(parent) {}

//void CustomDial::paintEvent(QPaintEvent *event) {
//    // 기본 다이얼 배경을 먼저 그립니다.
//    QDial::paintEvent(event);

//    QPainter painter(this);
//    painter.setRenderHint(QPainter::Antialiasing);

//    // 중앙 좌표 계산
//    int centerX = width() / 2;
//    int centerY = height() / 2;

//    // 텍스트 스타일 설정
//    QFont font = painter.font();
//    font.setBold(true);
//    font.setPointSize(20);
//    painter.setFont(font);

//    // 현재 값 텍스트 그리기
//    QString valueText = QString::number(value());
//    QRect textRect(centerX - 20, centerY - 10, 40, 20);
//    painter.drawText(textRect, Qt::AlignCenter, valueText);


//    // 다이얼 중심과 반경 계산
//    int cx = width() / 2;
//    int cy = height() / 2;
//    int radius = qMin(cx, cy) - 10;

//    // 현재 값에 따라 노브 각도 계산
//    double angle = (360.0 * (value() - minimum())) / (maximum() - minimum());
//    double radians = qDegreesToRadians(angle - 90);  // -90도는 12시 방향으로 이동

//    // 노브 위치 계산
//    int knobX = static_cast<int>(cx + radius * cos(radians));
//    int knobY = static_cast<int>(cy + radius * sin(radians));

//    // 삼각형 좌표 생성
//    QPolygon triangle;
//    triangle << QPoint(knobX, knobY)                    // 삼각형의 꼭짓점
//             << QPoint(knobX - 10, knobY - 15)          // 왼쪽 아래
//             << QPoint(knobX + 10, knobY - 15);         // 오른쪽 아래

//    // 삼각형 그리기
//    painter.setBrush(Qt::red);  // 삼각형 색상 설정
//    painter.drawPolygon(triangle);
//}
#include "CustomDial.h"
#include <QPainter>
#include <QResizeEvent>
#include <QIntValidator>
#include <QtMath>
#include <QDebug>

CustomDial::CustomDial(QWidget *parent)
    : QDial(parent),
      valueLabel(new QLabel(this)),
      lineEdit(new QLineEdit(this)) {
    // Wrapping 활성화
    setWrapping(true);

    // 범위 설정 (0 ~ 3600)
    setRange(0, 3600);
    setValue(1800);
    setFixedSize(350,350);

    // Label 설정
    valueLabel->setText(QString::number((value()-1800)/10));
    valueLabel->setAlignment(Qt::AlignCenter);
    valueLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: white; background: transparent;");

    // LineEdit 설정
    lineEdit->setValidator(new QIntValidator(minimum(), maximum(), this));
    lineEdit->setPlaceholderText("Enter value");
    lineEdit->setAlignment(Qt::AlignCenter);
    lineEdit->setStyleSheet("font-size: 14px;");

    // Signal-Slot 연결
    connect(this, &QDial::valueChanged, this, [this](int value) {
        valueLabel->setText(QString::number(((value-1800) / 10.0), 'f', 1));
        lineEdit->setText(QString::number(value));
    });

    connect(lineEdit, &QLineEdit::editingFinished, this, &CustomDial::onLineEditValueChanged);

    // 초기 위치 업데이트
    updateWidgetPositions();
}

void CustomDial::resizeEvent(QResizeEvent *event) {
    QDial::resizeEvent(event);
    updateWidgetPositions();
}


void CustomDial::paintEvent(QPaintEvent *event) {
    // 기본 QDial 드로잉
//    QDial::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 원의 중심과 반지름 계산
    int centerX = width() / 2;
    int centerY = height() / 2;
//    int radius = qMin(width(), height()) / 2 - 20;  // 다이얼 반지름
    // 원의 크기를 줄임
    int radius = qMin(width(), height()) / 3;  // 줄어든 원 반지름
    int radius2 = qMin(width(), height()) / 4;  // 줄어든 원 반지름
    QRectF reducedRect(centerX - radius, centerY - radius, radius * 2, radius * 2);

    QRectF reducedRect2(centerX - radius2, centerY - radius2, radius2 * 2, radius2 * 2);

    // 원 그리기
    QPen circlePen(Qt::white, 5); // 검정색, 두께 4
    painter.setBrush(Qt::NoBrush);
    painter.setPen(circlePen);
    painter.drawEllipse(reducedRect);
    painter.drawEllipse(reducedRect2);

    // 눈금 및 숫자 설정
    int range = maximum() - minimum();
    int tickLength = TICK_LENGTH;

    // 텍스트 글꼴 크기 설정
    QFont font = painter.font();
    font.setPointSize(15);  // 글씨 크기를 12포인트로 설정
    font.setBold(true);     // 굵게 설정
    painter.setFont(font);
    painter.setPen(Qt::white); // 텍스트 색상 설정 (어두운 회색)
    QPen tickPen(Qt::white, 2); // 검정색, 두께 4
    for (int i = 0; i <= range-1; i += SCALE_WIDTH) {  // 10 단위로 눈금 및 숫자 배치
        double angle = 90.0 - (i * 360 / range);  // 각도 계산
        double radian = qDegreesToRadians(angle);

        // 눈금 시작점과 끝점 계산
        int x1 = centerX + (radius * qCos(radian));
        int y1 = centerY - (radius * qSin(radian));
        int x2 = centerX + ((radius - tickLength) * qCos(radian));
        int y2 = centerY - ((radius - tickLength) * qSin(radian));

        // 숫자 그리기 (눈금 바깥)
        if (i % 200 == 0) {  // 숫자는 큰 눈금에만 표시
            int textRadius = radius + 20;  // 숫자 위치 반지름 (눈금 바깥쪽)
            int textX = centerX + (textRadius * qCos(radian));
            int textY = centerY - (textRadius * qSin(radian));
            qDebug()<<"text X : "<<textX<<", text Y : "<<textY;
            painter.setPen(tickPen); // 눈금 기본 검정색
            if(i < 1100){
                // 눈금 그리기
                QString text = QString::number(minimum() + (i/10));
                painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));
                painter.drawText(QRectF(textX - TEXT_AREA, textY - TEXT_AREA, 60, 50), Qt::AlignCenter, text);
            }else if(i>2590){
                // 눈금 그리기
                QString text = QString::number(minimum() + ((i-3600)/10));
                painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));
                painter.drawText(QRectF(textX - TEXT_AREA - 10, textY - TEXT_AREA, 60, 50), Qt::AlignCenter, text);
            }
        }
    }
    // 다이얼 핸들 그리기 (값에 따라 위치 조정)
    int startAngle = 270;
//    double handleAngle = startAngle - (value() * 360.0 / range);
//    double handleRadian = qDegreesToRadians(handleAngle);

//    int handleRadius = radius - 15;  // 핸들이 원 안쪽으로 배치되도록 설정
//    int handleX = centerX + (handleRadius * qCos(handleRadian));
//    int handleY = centerY - (handleRadius * qSin(handleRadian));

//    // 핸들 그리기
//    painter.setBrush(Qt::red);
//    painter.setPen(Qt::NoPen);
//    painter.drawEllipse(QPointF(handleX, handleY), 5, 5);
    // 핸들 위치 계산
    double handleAngle = startAngle - (value() * 360.0 / range);
    double handleRadian = qDegreesToRadians(handleAngle);

    int handleRadius = radius - 25;
    int handleX = centerX + (handleRadius * qCos(handleRadian));
    int handleY = centerY - (handleRadius * qSin(handleRadian));

    // 삼각형 꼭짓점 계산
    int triangleBase = 10;
    int triangleHeight = 20;

    QPointF p1(
        handleX + (triangleHeight * qCos(handleRadian)),
        handleY - (triangleHeight * qSin(handleRadian))
    ); // 삼각형 꼭짓점 (방향 따라)
    QPointF p2(
        handleX + (triangleBase / 2 * qCos(handleRadian + M_PI / 2)),
        handleY - (triangleBase / 2 * qSin(handleRadian + M_PI / 2))
    ); // 삼각형 왼쪽 점
    QPointF p3(
        handleX + (triangleBase / 2 * qCos(handleRadian - M_PI / 2)),
        handleY - (triangleBase / 2 * qSin(handleRadian - M_PI / 2))
    ); // 삼각형 오른쪽 점

    // 삼각형 그리기
    painter.setBrush(Qt::yellow);
    painter.setPen(Qt::NoPen);
    QPolygonF triangle;
    triangle << p1 << p2 << p3;
    painter.drawPolygon(triangle);
}


void CustomDial::updateWidgetPositions() {
    // Dial의 중앙 좌표 계산
    int centerX = width() / 2;
    int centerY = height() / 2;

    // Label 위치 설정 (다이얼의 중앙)
    int labelWidth = 120;
    int labelHeight = 50;
    valueLabel->setStyleSheet("font-size: 40px; font-weight: bold; color: yellow; background: transparent;");
    valueLabel->setGeometry(centerX - labelWidth / 2, centerY-50, labelWidth, labelHeight);

    // LineEdit 위치 설정 (Label 바로 아래)
    int lineEditWidth = 120;
    int lineEditHeight = 50;
    lineEdit->setStyleSheet("font-size: 40px; font-weight: bold; color: white; background: transparent;");
    lineEdit->setGeometry(centerX - lineEditWidth / 2, centerY+10, lineEditWidth, lineEditHeight);
}

void CustomDial::onLineEditValueChanged() {
    // LineEdit 값 읽기 및 다이얼 값 설정
    int newValue = lineEdit->text().toInt();
    if (newValue >= minimum() && newValue <= maximum()) {
        setValue(newValue);
    } else {
        lineEdit->setText(QString::number(value())); // 유효하지 않은 값은 기존 값 유지
    }
}

