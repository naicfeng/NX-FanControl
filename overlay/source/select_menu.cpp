#include "select_menu.hpp"
#include "utils.hpp"

SelectMenu::SelectMenu(int i, TemperaturePoint* fanCurveTable, bool* tableIsChanged)
{
    this->_i = i;
    this->_fanCurveTable = fanCurveTable;
    this->_tableIsChanged = tableIsChanged;

    this->_saveBtn = new tsl::elm::ListItem("保存调整");
    this->_tempLabel = new tsl::elm::CategoryHeader("核心温度: "+std::to_string((this->_fanCurveTable + this->_i)->temperature_c) + "℃", true);
    this->_fanLabel = new tsl::elm::CategoryHeader("风扇转速: "+std::to_string((int)((this->_fanCurveTable + this->_i)->fanLevel_f * 100)) + "%", true);
}

tsl::elm::Element* SelectMenu::createUI(){

    auto frame = new tsl::elm::OverlayFrame("风扇控制", "v1.0.5+");

    auto list = new tsl::elm::List();

    list->addItem(this->_tempLabel);
    auto stepTemp = new tsl::elm::StepTrackBar("温度(℃)", 21);
    stepTemp->setValueChangedListener([this](u8 value)
    {
        this->_tempLabel->setText("核心温度: "+std::to_string(value * 5) + "℃");
        (this->_fanCurveTable + this->_i)->temperature_c = value * 5;
        this->_saveBtn->setText("保存调整");
    });
    stepTemp->setProgress(((this->_fanCurveTable + this->_i)->temperature_c) / 5);
    list->addItem(stepTemp);

    list->addItem(this->_fanLabel);
    auto stepFanL = new tsl::elm::StepTrackBar("转速(%)", 21);
    stepFanL->setValueChangedListener([this](u8 value)
    {
        this->_fanLabel->setText("风扇转速: "+std::to_string(value * 5) + "%");
        (this->_fanCurveTable + this->_i)->fanLevel_f = (float)(value * 5)/100;
        this->_saveBtn->setText("保存调整");
    });
    stepFanL->setProgress(((int)((this->_fanCurveTable + this->_i)->fanLevel_f * 100)) / 5);
    list->addItem(stepFanL);

    this->_saveBtn->setClickListener([this](uint64_t keys)
    {
	    if (keys & HidNpadButton_A)
        {
		    WriteConfigFile(this->_fanCurveTable);

            if(IsRunning() != 0)
            {
                pmshellTerminateProgram(SysFanControlID);
                const NcmProgramLocation programLocation
                {
                    .program_id = SysFanControlID,
                    .storageID = NcmStorageId_None,
                };
                u64 pid = 0;
                pmshellLaunchProgram(0, &programLocation, &pid);
            }

            this->_saveBtn->setText("保存成功");
            *this->_tableIsChanged = true;
		    return true;
		}

        return false;

    });

    list->addItem(this->_saveBtn);

    frame->setContent(list);

    return frame;
}