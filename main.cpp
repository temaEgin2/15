#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QFrame>
#include <QFile>
#include <QTextStream>
#include <QPushButton>
#include <QPainter>
const int NUM_CELLS = 8;

class BitCell : public QLabel {
public:
    BitCell(QWidget *parent = nullptr) : QLabel(parent) {}

    void setValue(bool value) {
        m_value = value;
        update();
    }

    bool value() const { return m_value; }

protected:
    void paintEvent(QPaintEvent *event) override {
        QLabel::paintEvent(event);
        QPainter painter(this);
        painter.setPen(Qt::black);
        painter.setFont(QFont("Arial", 12));
        painter.drawText(rect(), Qt::AlignCenter, m_value ? "1" : "0");
    }

private:
    bool m_value = false;
};

class BitMapWidget : public QWidget {
public:
    BitMapWidget(QWidget *parent = nullptr) : QWidget(parent) {
        setFixedSize(400, 80);
        setWindowTitle("BitMap v0.1");

        m_cells.resize(NUM_CELLS);
        for (int i = 0; i < NUM_CELLS; i++) {
            m_cells[i] = new BitCell(this);
            m_cells[i]->setFixedSize(40, 40);
        }

        QPushButton *reloadButton = new QPushButton("Reload", this);
        connect(reloadButton, &QPushButton::clicked, this, &BitMapWidget::rel);

        QVBoxLayout *layout = new QVBoxLayout;
        QHBoxLayout *cellsLayout = new QHBoxLayout;
        cellsLayout->setSpacing(5);
        cellsLayout->setMargin(0);
        for (int i = 0; i < NUM_CELLS; i++) {
            cellsLayout->addWidget(m_cells[i]);
        }
        layout->addLayout(cellsLayout);
        layout->addWidget(reloadButton);
        setLayout(layout);

        rel();
    }

    void rel() {
        QFile file("config.ini");
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            QString line = in.readLine();
            if (!line.isEmpty()) {
                bool ok = false;
                quint8 byte = line.toUInt(&ok, 16);
                if (ok) {
                    for (int i = 0; i < NUM_CELLS; i++) {
                        m_cells[i]->setValue(byte & (1 << (NUM_CELLS - i - 1)));
                    }
                }
            }
            file.close();
        }
    }

private:
    QVector<BitCell*> m_cells;
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    BitMapWidget w;
    w.show();

    return a.exec();
}
