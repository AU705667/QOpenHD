#include <QQuickItem>
#include <QQuickPaintedItem>
#include <QPainter>

#include "openhd.h"

class HeadingLadder : public QQuickPaintedItem {
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(bool showHeadingLadderText MEMBER m_showHeadingLadderText WRITE setShowHeadingLadderText NOTIFY showHeadingLadderTextChanged)
    Q_PROPERTY(bool imperial MEMBER m_imperial WRITE setImperial NOTIFY imperialChanged)
    Q_PROPERTY(bool showHorizonHome MEMBER m_showHorizonHome WRITE setShowHorizonHome NOTIFY showHorizonHomeChanged)
    Q_PROPERTY(bool showHorizonHeadingLadder MEMBER m_showHorizonHeadingLadder WRITE setShowHorizonHeadingLadder NOTIFY showHorizonHeadingLadderChanged)
    QML_ELEMENT

//show_horizon_heading_ladder

public:
    explicit HeadingLadder(QQuickItem* parent = nullptr);

    void paint(QPainter* painter) override;

    QColor color() const;

public slots:
    void setColor(QColor color);
    void setShowHeadingLadderText(bool showHeadingLadderText);
    void setImperial(bool imperial);
    void setShowHorizonHome(bool showHorizonHome);
    void setShowHorizonHeadingLadder(bool showHorizonHeadingLadder);

signals:
    void colorChanged(QColor color);
    void imperialChanged(bool imperial);
    void showHeadingLadderTextChanged(bool showHeadingLadderText);
    void showHorizonHomeChanged(bool showHorizonHome);
    void showHorizonHeadingLadderChanged(bool showHorizonHeadingLadder);

private:
    QColor m_color;
    bool m_showHeadingLadderText;
    bool m_imperial;
    bool m_showHorizonHome;
    bool m_showHorizonHeadingLadder;
};
