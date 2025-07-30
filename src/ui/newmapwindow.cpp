#include "newmapwindow.h"

#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

NewMapWindow::NewMapWindow(QWidget *parent)
    : QDialog{parent}
{
    setWindowTitle("创建新地图");

    auto imageLabel = new QLabel("", this);
    auto imageButton = new QPushButton("选择参考图", this);
    auto nameLabel = new QLabel("名称", this);
    auto nameInput = new QLineEdit(this);
    auto scaleLabel = new QLabel("比例", this);
    auto scaleSpinBox = new QDoubleSpinBox(this);
    scaleSpinBox->setSingleStep(0.1);
    scaleSpinBox->setDecimals(3);
    scaleSpinBox->setValue(1.0);

    auto buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal, this);
    buttonBox->setEnabled(false);

    auto updateButtonBox = [=, this]() {
        if (m_imageFilePath.isNull() || nameInput->text().isNull()) {
            buttonBox->setEnabled(false);
        } else {
            buttonBox->setEnabled(true);
        }
    };

    connect(imageButton, &QPushButton::clicked, [=, this]() {
        QString filePath = QFileDialog::getOpenFileName(
            this,
            "选择参考图",
            "",
            "Images (*.png *.jpg *.bmp *.gif)"
            );
        if (!filePath.isNull()) {
            m_imageFilePath = filePath;
        }
        updateButtonBox();
        imageLabel->setText(m_imageFilePath);
    });

    connect(nameInput, &QLineEdit::textChanged, updateButtonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, [=, this]() {
        m_mapTitle = nameInput->text();
        m_scale = scaleSpinBox->value();
        QDialog::accept();
    });

    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    auto layout = new QGridLayout(this);
    layout->addWidget(imageLabel, 1, 1);
    layout->addWidget(imageButton, 1, 2);
    layout->addWidget(nameLabel, 2, 1);
    layout->addWidget(nameInput, 2, 2);
    layout->addWidget(scaleLabel, 3, 1);
    layout->addWidget(scaleSpinBox, 3, 2);
    layout->addWidget(buttonBox, 4, 1, 1, 2);
}
