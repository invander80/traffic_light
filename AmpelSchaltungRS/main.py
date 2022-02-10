# // PYQT5 IMPORTS
from PyQt5.QtCore import pyqtSlot, QIODevice
from PyQt5.QtGui import QColor
from PyQt5.QtWidgets import QApplication, QWidget, QPlainTextEdit, QGraphicsDropShadowEffect
from PyQt5.QtSerialPort import QSerialPort, QSerialPortInfo


import time
# // UI IMPORT
from ui.ui_main import Ui_Form
from customs import CodeEditor


# // AMPELSCHALTUNG MAIN KLASSE
class AmpelSchaltungRS(QWidget,Ui_Form):
    def __init__(self):
        super(AmpelSchaltungRS, self).__init__()
        self.setupUi(self)  #=> UI init

        self.r_taster.setCheckable(True)
        self.s_taster.setCheckable(True)
        self.ampel_taster.setCheckable(True)

        self.plain: QPlainTextEdit = CodeEditor(self)
        self.plain.setReadOnly(True)
        self.verticalLayout.addWidget(self.plain)

        self.schatten_setzen(self.ampelFrame)
        self.schatten_setzen(self.ampel_taster)
        self.schatten_setzen(self.plain)

        self.arduino: QSerialPort = QSerialPort('/dev/tty.usbmodem14201',
                                                baudRate=QSerialPort.Baud9600,
                                                readyRead=self.onRead)
        self.onOpen()

        """self.s_taster.clicked.connect(self.on_s_taster_clicked)
        self.r_taster.clicked.connect(self.on_r_taster_clicked)
        self.ampel_taster.clicked.connect(self.on_ampel_taster_clicked)"""

        self.phase_an: list = ["Ampel-Grün an", "Ampel-Gelb an", "Ampel-Rot an", "Fußgänger-Rot an", "Fußgänger-Gelb an"]
        self.phase_aus = ["Ampel-Grün aus", "Ampel-Gelb aus", "Ampel-Rot aus", "Fußgänger-Rot aus", "Fußgänger-Gelb aus"]
        self.ampel_farben: list = ["#7bffa0", "#ffff7f", "#ff554f", "#ff554f", "#ffff7f"]
        self.leds: list = [self.ampelGruen, self.ampelGelb, self.ampelRot, self.fussRot, self.fussGelb]

    @pyqtSlot(bool)
    def onOpen(self):
        self.arduino.setPortName("COM4")
        self.arduino.open(QIODevice.ReadWrite)

    def onRead(self):
        while self.arduino.canReadLine():
            serialpl = self.arduino.readLine().data().decode()
            serialpl = serialpl.rstrip('\r\n')
            self.ampelPhasen(serialpl)
            self.r_taster_state(serialpl)
            self.s_taster_state(serialpl)
            self.ampel_taster_state(serialpl)
            self.plain.appendPlainText(f"{self.uhrzeit()} : {serialpl}")

    def ampelPhasen(self, val):
        for x in range(len(self.phase_an)):
            if self.phase_an[x] in val:
                self.leds[x].setStyleSheet(f"background:{self.ampel_farben[x]};")
            if self.phase_aus[x] in val:
                self.leds[x].setStyleSheet("background:none;")

    def r_taster_state(self, val):
        if "R-Taster an" in val:
            self.r_taster.setChecked(True)
        if "R-Taster aus" in val:
            self.r_taster.setChecked(False)

    def s_taster_state(self, val):
        if "S-Taster an" in val:
            self.s_taster.setChecked(True)
        if "S-Taster aus" in val:
            self.s_taster.setChecked(False)

    def ampel_taster_state(self, val):
        if "Ampel-Taster betätigt" in val:
            self.ampel_taster.setChecked(True)
        if "9" in val:
            self.ampel_taster.setChecked(False)

    # // Aktuelle Uhrzeit
    def uhrzeit(self) -> str:
        t = time.localtime()
        return time.strftime("%H:%M:%S", t)

    # // Schatten
    def schatten_setzen(self, widget):
        schatten = QGraphicsDropShadowEffect(widget)
        schatten.setXOffset(0)
        schatten.setYOffset(0)
        schatten.setBlurRadius(20)
        schatten.setColor(QColor("white"))
        widget.setGraphicsEffect(schatten)


if __name__ == '__main__':
    import sys
    app = QApplication(sys.argv)
    asRS = AmpelSchaltungRS()
    asRS.show()
    app.exec()
