#include "CustomDial.h"
#include <QPainter>
#include <QResizeEvent>
#include <QIntValidator>
#include <QtMath>
#include <QDebug>

CustomDial::CustomDial(QWidget *parent)
    : QDial(parent),
      lbAngleValue(new QLabel(this)),
      leSetAngle(new QLineEdit(this)) {
    // Wrapping 활성화
    setWrapping(true);

    // 범위 설정 (0 ~ 3600)
    setRange(0, 3600);
    setValue(1800);
    setFixedSize(350,350);

    // Label 설정
    lbAngleValue->setText(QString::number((value()-1800)/10));
    lbAngleValue->setAlignment(Qt::AlignCenter);
    lbAngleValue->setStyleSheet("font-size: 16px; font-weight: bold; color: white; background: transparent;");

    // LineEdit 설정
    leSetAngle->setValidator(new QIntValidator(minimum(), maximum(), this));
    leSetAngle->setPlaceholderText("Set Angle");
    leSetAngle->setAlignment(Qt::AlignCenter);
    leSetAngle->setStyleSheet("font-size: 14px;");

    // Signal-Slot 연결
    connect(this, &QDial::valueChanged, this, [this](int value) {
        lbAngleValue->setText(QString::number(((value-1800) / 10.0), 'f', 1));
        leSetAngle->setText(QString::number(value));
    });

    connect(leSetAngle, &QLineEdit::editingFinished, this, &CustomDial::onLineEditValueChanged);

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
    int iCircleCenterX = width()/2;
    int iCircleCenterY = height()/2;
    int iOuterCircleRadius = qMin(width(), height())/3;
    int iInnerCircleRadius  = qMin(width(), height())/4;

    // 원의 크기를 줄임
//    int radius = qMin(width(), height()) / 3;  // 줄어든 원 반지름
//    int radius2 = qMin(width(), height()) / 4;  // 줄어든 원 반지름
    QRectF outerRect(iCircleCenterX - iOuterCircleRadius, iCircleCenterY - iOuterCircleRadius, iOuterCircleRadius*2, iOuterCircleRadius*2);
    QRectF innerRect(iCircleCenterX - iInnerCircleRadius, iCircleCenterY - iInnerCircleRadius, iInnerCircleRadius*2, iInnerCircleRadius*2);

    // 원 그리기
    QPen circlePen(Qt::white, 5); // 검정색, 두께 4
    painter.setBrush(Qt::NoBrush);
    painter.setPen(circlePen);
    painter.drawEllipse(outerRect);
    painter.drawEllipse(innerRect);

    // 눈금 및 숫자 설정
    int iAngleRange = maximum() - minimum();
    int iTickLength = TICK_LENGTH;

    // 텍스트 글꼴 크기 설정
    QFont font = painter.font();
    font.setPointSize(15);  // 글씨 크기를 15포인트로 설정
    font.setBold(true);     // 굵게 설정
    painter.setFont(font);
    painter.setPen(Qt::white); // 텍스트 색상 설정 (어두운 회색)
    QPen tickPen(Qt::white, 3); // 검정색, 두께 4
    for (int i = 0; i <= iAngleRange-1; i += SCALE_WIDTH) {  // 10 단위로 눈금 및 숫자 배치
        double angle = 90.0 - (i * 360 / iAngleRange);  // 각도 계산
        double radian = qDegreesToRadians(angle);

        // 눈금 시작점과 끝점 계산 문서 작성 필요
        int x1 = iCircleCenterX + (iOuterCircleRadius * qCos(radian));
        int y1 = iCircleCenterY - (iOuterCircleRadius * qSin(radian));
        int x2 = iCircleCenterX + ((iOuterCircleRadius - iTickLength) * qCos(radian));
        int y2 = iCircleCenterY - ((iOuterCircleRadius - iTickLength) * qSin(radian));

        // 숫자 그리기 (눈금 바깥)
        if (i % 200 == 0) {  // 숫자는 큰 눈금에만 표시
            int textRadius = iOuterCircleRadius + 20;  // 숫자 위치 반지름 (눈금 바깥쪽)
            int textX = iCircleCenterX + (textRadius * qCos(radian));
            int textY = iCircleCenterY - (textRadius * qSin(radian));
            qDebug()<<"text X : "<<textX<<", text Y : "<<textY;
            painter.setPen(tickPen); // 눈금 기본 검정색
            if(i < 1100){
                // 눈금 그리기
//                QString textAz = "AZ";
//                painter.drawText(QRectF(textX - TEXT_AREA, textY - TEXT_AREA, 70, 50), Qt::AlignCenter, textAz);
                QString text = QString::number(minimum() + (i/10));
                painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));
                painter.drawText(QRectF(textX - TEXT_AREA, textY - TEXT_AREA, 70, 50), Qt::AlignCenter, text);
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
    // 핸들 위치 계산
    double handleAngle = startAngle - (value() * 360.0 / iAngleRange);
    double handleRadian = qDegreesToRadians(handleAngle);

    int handleRadius = iOuterCircleRadius - 25;
    int handleX = iCircleCenterX + (handleRadius * qCos(handleRadian));
    int handleY = iCircleCenterY - (handleRadius * qSin(handleRadian));

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
    lbAngleValue->setStyleSheet("font-size: 40px; font-weight: bold; color: yellow; background: transparent;");
    lbAngleValue->setGeometry(centerX - labelWidth / 2, centerY-50, labelWidth, labelHeight);

    // LineEdit 위치 설정 (Label 바로 아래)
    int lineEditWidth = 120;
    int lineEditHeight = 50;
    leSetAngle->setStyleSheet("font-size: 40px; font-weight: bold; color: white; background: transparent;");
    leSetAngle->setGeometry(centerX - lineEditWidth / 2, centerY+10, lineEditWidth, lineEditHeight);
}

void CustomDial::onLineEditValueChanged() {
    // LineEdit 값 읽기 및 다이얼 값 설정
    int newValue = leSetAngle->text().toInt();
    if (newValue >= minimum() && newValue <= maximum()) {
        setValue(newValue);
    } else {
        leSetAngle->setText(QString::number(value())); // 유효하지 않은 값은 기존 값 유지
    }
}

