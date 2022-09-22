// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef QSGVideoTextureItem_H
#define QSGVideoTextureItem_H

#include <QtQuick/QQuickItem>
#include <QtQuick/QQuickWindow>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QTimer>

#include "texturerenderer.h"

#include "avcodec_decoder.h"

// Hoock into the QT Scene graph and draw video directly without the "intermediate" rgba texture as
// required with (for examle) qmlglsink. See the qt "Squircle" documentation
class QSGVideoTextureItem : public QQuickItem
{
    Q_OBJECT
    QML_ELEMENT
public:
    QSGVideoTextureItem();

public slots:
    void sync();
    void cleanup();

private slots:
    void handleWindowChanged(QQuickWindow *win);

private:
    void releaseResources() override;

    TextureRenderer* m_renderer;
public slots:
    void m_QQuickWindow_beforeRendering();
    void m_QQuickWindow_beforeRenderPassRecording();
private:
    std::unique_ptr<AVCodecDecoder> m_av_codec_decoder=nullptr;
};

#endif // QSGVideoTextureItem_H
