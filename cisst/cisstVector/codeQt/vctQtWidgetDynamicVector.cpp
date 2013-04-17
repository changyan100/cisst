/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id$

  Author(s):  Anton Deguet
  Created on: 2011-12-08

  (C) Copyright 2011-2013 Johns Hopkins University (JHU), All Rights
  Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---

*/

#include <cisstVector/vctQtWidgetDynamicVector.h>

#include <QTableWidget>
#include <QHeaderView>
#include <QSpinBox>
#include <QDoubleSpinBox>


vctQtWidgetDynamicVectorReadBase::vctQtWidgetDynamicVectorReadBase(void):
    Table(0)
{
    this->Table = new QTableWidget();
    this->Table->setRowCount(1);
    this->Table->verticalHeader()->hide();
    this->Table->horizontalHeader()->hide();
    this->Table->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    this->Table->verticalHeader()->setResizeMode(QHeaderView::Stretch);
    int verticalHeight =
        this->Table->horizontalHeader()->sizeHint().height()
        + this->Table->verticalHeader()->sizeHint().height();
    this->Table->setFixedHeight(verticalHeight);
}

QWidget * vctQtWidgetDynamicVectorReadBase::GetWidget(void)
{
    return this->Table;
}


template <class _elementType>
vctQtWidgetDynamicVectorReadFloating<_elementType>::vctQtWidgetDynamicVectorReadFloating(void):
    vctQtWidgetDynamicVectorReadBase(),
    Precision(2),
    Format('f')
{
}

template <class _elementType>
void vctQtWidgetDynamicVectorReadFloating<_elementType>::SetPrecision(const int precision)
{
    Precision = precision;
}

template <class _elementType>
void vctQtWidgetDynamicVectorReadFloating<_elementType>::SetFormat(const char format)
{
    Format = format;
}

template <class _elementType>
bool vctQtWidgetDynamicVectorReadFloating<_elementType>::SetValue(const vctDynamicVector<_elementType> & vector)
{
    const size_t size = vector.size();
    if (static_cast<size_t>(this->Table->columnCount()) != size) {
        this->Table->setColumnCount(size);
    }
    QTableWidgetItem * tableItem;
    for (size_t index = 0; index < size; ++index) {
        tableItem = this->Table->item(0, index);
        if (tableItem == 0) {
            tableItem = new QTableWidgetItem();
            tableItem->setTextAlignment(Qt::AlignRight);
            tableItem->setFlags(tableItem->flags() ^ Qt::ItemIsEditable);
            this->Table->setItem(0, index, tableItem);
        }
        tableItem->setText(QString::number(vector.Element(index), Format, Precision));
    }
    return true;
}

template <class _elementType>
vctQtWidgetDynamicVectorReadInteger<_elementType>::vctQtWidgetDynamicVectorReadInteger(void):
    vctQtWidgetDynamicVectorReadBase(),
    Base(10)
{
}

template <class _elementType>
void vctQtWidgetDynamicVectorReadInteger<_elementType>::SetBase(const int base)
{
    Base = base;
}


template <class _elementType>
bool vctQtWidgetDynamicVectorReadInteger<_elementType>::SetValue(const vctDynamicVector<_elementType> & vector)
{
    const size_t size = vector.size();
    if (static_cast<size_t>(this->Table->columnCount()) != size) {
        this->Table->setColumnCount(size);
    }
    QTableWidgetItem * tableItem;
    for (size_t index = 0; index < size; ++index) {
        tableItem = this->Table->item(0, index);
        if (tableItem == 0) {
            tableItem = new QTableWidgetItem();
            tableItem->setTextAlignment(Qt::AlignRight);
            tableItem->setFlags(tableItem->flags() ^ Qt::ItemIsEditable);
            this->Table->setItem(0, index, tableItem);
        }
        tableItem->setText(QString::number(vector.Element(index), Base));
    }
    return true;
}


template class vctQtWidgetDynamicVectorReadFloating<double>;
template class vctQtWidgetDynamicVectorReadFloating<float>;
template class vctQtWidgetDynamicVectorReadInteger<int>;
template class vctQtWidgetDynamicVectorReadInteger<unsigned int>;
template class vctQtWidgetDynamicVectorReadInteger<bool>;


vctQtWidgetDynamicVectorWriteBase::vctQtWidgetDynamicVectorWriteBase(const DisplayModeType displayMode):
    DisplayMode(displayMode)
{
    this->Table = new QTableWidget();
    this->Table->setRowCount(1);
    this->Table->verticalHeader()->hide();
    this->Table->horizontalHeader()->hide();
    this->Table->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    this->Table->verticalHeader()->setResizeMode(QHeaderView::Stretch);
    int verticalHeight =
        this->Table->horizontalHeader()->sizeHint().height()
        + this->Table->verticalHeader()->sizeHint().height();
    this->Table->setFixedHeight(verticalHeight);
    connect(this->Table, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(ItemChangedSlot(QTableWidgetItem *)));
}

QWidget * vctQtWidgetDynamicVectorWriteBase::GetWidget(void)
{
    return this->Table;
}

void vctQtWidgetDynamicVectorWriteBase::SliderValueChangedSlot(int CMN_UNUSED(value))
{
    emit this->valueChanged();
}

void vctQtWidgetDynamicVectorWriteBase::DoubleSpinBoxValueChangedSlot(double CMN_UNUSED(value))
{
    emit this->valueChanged();
}

void vctQtWidgetDynamicVectorWriteBase::SpinBoxValueChangedSlot(int CMN_UNUSED(value))
{
    emit this->valueChanged();
}

void vctQtWidgetDynamicVectorWriteBase::ItemChangedSlot(QTableWidgetItem * CMN_UNUSED(item))
{
    emit this->valueChanged();
}

template <class _elementType>
vctQtWidgetDynamicVectorWriteFloating<_elementType>::vctQtWidgetDynamicVectorWriteFloating(const DisplayModeType displayMode):
    vctQtWidgetDynamicVectorWriteBase(displayMode),
    Precision(2),
    Format('f'),
    Minimum(-100.0),
    Maximum(100.0),
    Step(0.01)
{
}

template <class _elementType>
void vctQtWidgetDynamicVectorWriteFloating<_elementType>::SetPrecision(const int precision)
{
    Precision = precision;
}

template <class _elementType>
void vctQtWidgetDynamicVectorWriteFloating<_elementType>::SetFormat(const char format)
{
    Format = format;
}

template <class _elementType>
void vctQtWidgetDynamicVectorWriteFloating<_elementType>::SetRange(const value_type minimum,
                                                                   const value_type maximum)
{
    Minimum = minimum;
    Maximum = maximum;
}

template <class _elementType>
void vctQtWidgetDynamicVectorWriteFloating<_elementType>::SetStep(const value_type step)
{
    Step = step;
}

template <class _elementType>
bool vctQtWidgetDynamicVectorWriteFloating<_elementType>::SetValue(const vctDynamicVector<value_type> & vector, bool blockSignals)
{
    const bool previousBlockSignals = this->blockSignals(blockSignals);

    const size_t size = vector.size();
    switch (DisplayMode) {
    case TEXT_WIDGET:
        if (static_cast<size_t>(this->Table->columnCount()) != size) {
            this->Table->setColumnCount(size);
        }
        QTableWidgetItem * tableItem;
        for (size_t index = 0; index < size; ++index) {
            tableItem = this->Table->item(0, index);
            if (tableItem == 0) {
                tableItem = new QTableWidgetItem();
                tableItem->setTextAlignment(Qt::AlignRight);
                this->Table->setItem(0, index, tableItem);
            }
            tableItem->setText(QString::number(vector.Element(index), Format, Precision));
        }
        break;
    case SPINBOX_WIDGET:
        if (static_cast<size_t>(this->Table->columnCount()) != size) {
            this->Table->setColumnCount(size);
        }
        QDoubleSpinBox * spinBox;
        for (size_t index = 0; index < size; ++index) {
            spinBox = dynamic_cast<QDoubleSpinBox*>(this->Table->cellWidget(0, index));
            if (spinBox == 0) {
                spinBox = new QDoubleSpinBox();
                spinBox->setRange(Minimum, Maximum);
                spinBox->setSingleStep(Step);
                connect(spinBox, SIGNAL(valueChanged(double)), this, SLOT(DoubleSpinBoxValueChangedSlot(double)));
                this->Table->setCellWidget(0, index, spinBox);
            }
            spinBox->setValue(vector.Element(index));
        }
        break;
    case SLIDER_WIDGET:
        if (static_cast<size_t>(this->Table->columnCount()) != size) {
            this->Table->setColumnCount(size);
        }
        QSlider * slider;
        for (size_t index = 0; index < size; ++index) {
            slider = dynamic_cast<QSlider*>(this->Table->cellWidget(0, index));
            if (slider == 0) {
                slider = new QSlider(Qt::Horizontal);
                slider->setRange(0, SLIDER_RESOLUTION);
                slider->setSingleStep(1);
                connect(slider, SIGNAL(valueChanged(int)), this, SLOT(SliderValueChangedSlot(int)));
                this->Table->setCellWidget(0, index, slider);
            }
            slider->setValue((vector.Element(index) - Minimum) / (Maximum - Minimum) * SLIDER_RESOLUTION);
        }
        break;
    default:
        break;
    }

    this->blockSignals(previousBlockSignals);
    return true;
}

template <class _elementType>
bool vctQtWidgetDynamicVectorWriteFloating<_elementType>::GetValue(vctDynamicVector<value_type> & placeHolder) const
{
    const int columns = this->Table->columnCount();
    if (columns != static_cast<int>(placeHolder.size())) {
        return false;
    }
    switch (DisplayMode) {
    case TEXT_WIDGET:
        QTableWidgetItem * item;
        for (int column = 0; column < columns; ++column) {
            item = dynamic_cast<QTableWidgetItem*>(this->Table->item(0, column));
            placeHolder.Element(column) = item->text().toDouble();
        }
        break;
        break;
    case SPINBOX_WIDGET:
        QDoubleSpinBox * spinBox;
        for (int column = 0; column < columns; ++column) {
            spinBox = dynamic_cast<QDoubleSpinBox*>(this->Table->cellWidget(0, column));
            placeHolder.Element(column) = spinBox->value();
        }
        break;
    case SLIDER_WIDGET:
        QSlider * slider;
        for (int column = 0; column < columns; ++column) {
            slider = dynamic_cast<QSlider*>(this->Table->cellWidget(0, column));
            placeHolder.Element(column) = static_cast<double>(slider->value()) / SLIDER_RESOLUTION * (Maximum - Minimum) + Minimum;
        }
        break;
    default:
        break;
    }
    return true;
}

template class vctQtWidgetDynamicVectorWriteFloating<double>;
template class vctQtWidgetDynamicVectorWriteFloating<float>;



template <class _elementType>
vctQtWidgetDynamicVectorWriteInteger<_elementType>::vctQtWidgetDynamicVectorWriteInteger(const DisplayModeType displayMode):
    vctQtWidgetDynamicVectorWriteBase(displayMode),
    Base(10),
    Minimum(-100),
    Maximum(100),
    Step(1)
{
}

template <class _elementType>
void vctQtWidgetDynamicVectorWriteInteger<_elementType>::SetBase(const int base)
{
    Base = base;
}

template <class _elementType>
void vctQtWidgetDynamicVectorWriteInteger<_elementType>::SetRange(const value_type minimum,
                                                                  const value_type maximum)
{
    Minimum = minimum;
    Maximum = maximum;
}

template <class _elementType>
void vctQtWidgetDynamicVectorWriteInteger<_elementType>::SetStep(const value_type step)
{
    Step = step;
}

template <class _elementType>
bool vctQtWidgetDynamicVectorWriteInteger<_elementType>::SetValue(const vctDynamicVector<value_type> & vector, bool blockSignals)
{
    const bool previousBlockSignals = this->blockSignals(blockSignals);

    const size_t size = vector.size();
    switch (DisplayMode) {
    case TEXT_WIDGET:
        if (static_cast<size_t>(this->Table->columnCount()) != size) {
            this->Table->setColumnCount(size);
        }
        QTableWidgetItem * tableItem;
        for (size_t index = 0; index < size; ++index) {
            tableItem = this->Table->item(0, index);
            if (tableItem == 0) {
                tableItem = new QTableWidgetItem();
                tableItem->setTextAlignment(Qt::AlignRight);
                this->Table->setItem(0, index, tableItem);
            }
            tableItem->setText(QString::number(vector.Element(index), Base));
        }
        break;
    case SPINBOX_WIDGET:
        if (static_cast<size_t>(this->Table->columnCount()) != size) {
            this->Table->setColumnCount(size);
        }
        QSpinBox * spinBox;
        for (size_t index = 0; index < size; ++index) {
            spinBox = dynamic_cast<QSpinBox*>(this->Table->cellWidget(0, index));
            if (spinBox == 0) {
                spinBox = new QSpinBox();
                spinBox->setRange(Minimum, Maximum);
                spinBox->setSingleStep(Step);
                connect(spinBox, SIGNAL(valueChanged(int)), this, SLOT(SpinBoxValueChangedSlot(int)));
                this->Table->setCellWidget(0, index, spinBox);
            }
            spinBox->setValue(vector.Element(index));
        }
        break;
    case SLIDER_WIDGET:
        if (static_cast<size_t>(this->Table->columnCount()) != size) {
            this->Table->setColumnCount(size);
        }
        QSlider * slider;
        for (size_t index = 0; index < size; ++index) {
            slider = dynamic_cast<QSlider*>(this->Table->cellWidget(0, index));
            if (slider == 0) {
                slider = new QSlider(Qt::Horizontal);
                slider->setRange(Minimum, Maximum);
                slider->setSingleStep(Step);
                connect(slider, SIGNAL(valueChanged(int)), this, SLOT(SliderValueChangedSlot(int)));
                this->Table->setCellWidget(0, index, slider);
            }
            slider->setValue(vector.Element(index));
        }
        break;
    default:
        break;
    }

    this->blockSignals(previousBlockSignals);
    return true;
}

template <class _elementType>
bool vctQtWidgetDynamicVectorWriteInteger<_elementType>::GetValue(vctDynamicVector<value_type> & placeHolder) const
{
    const int columns = this->Table->columnCount();
    if (columns != static_cast<int>(placeHolder.size())) {
        return false;
    }
    switch (DisplayMode) {
    case TEXT_WIDGET:
        QTableWidgetItem * item;
        for (int column = 0; column < columns; ++column) {
            item = dynamic_cast<QTableWidgetItem*>(this->Table->item(0, column));
            placeHolder.Element(column) = item->text().toDouble();
        }
        break;
        break;
    case SPINBOX_WIDGET:
        QSpinBox * spinBox;
        for (int column = 0; column < columns; ++column) {
            spinBox = dynamic_cast<QSpinBox*>(this->Table->cellWidget(0, column));
            placeHolder.Element(column) = spinBox->value();
        }
        break;
    case SLIDER_WIDGET:
        QSlider * slider;
        for (int column = 0; column < columns; ++column) {
            slider = dynamic_cast<QSlider*>(this->Table->cellWidget(0, column));
            placeHolder.Element(column) = slider->value();
        }
        break;
    default:
        break;
    }
    return true;
}

template class vctQtWidgetDynamicVectorWriteInteger<int>;
template class vctQtWidgetDynamicVectorWriteInteger<unsigned int>;


vctQtWidgetDynamicVectorBoolWrite::vctQtWidgetDynamicVectorBoolWrite(void):
    QTableWidget()
{
    this->setRowCount(1);
    this->verticalHeader()->hide();
    this->horizontalHeader()->hide();
    this->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    this->verticalHeader()->setResizeMode(QHeaderView::Stretch);
    int verticalHeight = this->horizontalHeader()->sizeHint().height() + this->verticalHeader()->sizeHint().height();
    this->setFixedHeight(verticalHeight);
}

bool vctQtWidgetDynamicVectorBoolWrite::SetValue(const vctDynamicVector<bool> & vector)
{
    const size_t size = vector.size();
    if (static_cast<size_t>(this->columnCount()) != size) {
        this->setColumnCount(size);
    }
    QSpinBox * spinBox;
    for (size_t index = 0; index < size; ++index) {
        spinBox = dynamic_cast<QSpinBox*>(this->cellWidget(0, index));
        if (spinBox == 0) {
            spinBox = new QSpinBox();
            this->setCellWidget(0, index, spinBox);
            spinBox->setMinimum(0);
            spinBox->setMaximum(1);
        }
        spinBox->setValue(vector.Element(index));
    }
    return true;
}

bool vctQtWidgetDynamicVectorBoolWrite::GetValue(vctDynamicVector<bool> & placeHolder) const
{
    int columns = this->columnCount();
    vctDynamicVector<double> newVector(columns);
    QSpinBox * spinBox;
    for(int column = 0; column < columns; ++column) {
        spinBox = dynamic_cast<QSpinBox*>(this->cellWidget(1, column));
        newVector.Element(column) = (bool)spinBox->value();
    }
    placeHolder.Assign(newVector);
    return true;
}
