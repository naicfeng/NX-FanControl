#include "main_menu.hpp"
#include "select_menu.hpp"

MainMenu::MainMenu()
{
    ReadConfigFile(&this->_fanCurveTable);

    // Initialize sensors
    InitializeSensors();

    // Initialize temperature and fan speed labels
    this->_socTempLabel = new tsl::elm::ListItem("核心温度: --℃");
    this->_fanSpeedLabel = new tsl::elm::ListItem("风扇转速: --%");

    this->_p0Label = new tsl::elm::ListItem("一级: " + std::to_string(this->_fanCurveTable->temperature_c) + "℃ | " + std::to_string((int)(this->_fanCurveTable->fanLevel_f * 100)) + "%");
    this->_p1Label = new tsl::elm::ListItem("二级: " + std::to_string((this->_fanCurveTable + 1)->temperature_c) + "℃ | " + std::to_string((int)((this->_fanCurveTable + 1)->fanLevel_f * 100)) + "%");
    this->_p2Label = new tsl::elm::ListItem("三级: " + std::to_string((this->_fanCurveTable + 2)->temperature_c) + "℃ | " + std::to_string((int)((this->_fanCurveTable + 2)->fanLevel_f * 100)) + "%");
    this->_p3Label = new tsl::elm::ListItem("四级: " + std::to_string((this->_fanCurveTable + 3)->temperature_c) + "℃ | " + std::to_string((int)((this->_fanCurveTable + 3)->fanLevel_f * 100)) + "%");
    this->_p4Label = new tsl::elm::ListItem("五级: " + std::to_string((this->_fanCurveTable + 4)->temperature_c) + "℃ | " + std::to_string((int)((this->_fanCurveTable + 4)->fanLevel_f * 100)) + "%");
    this->_p5Label = new tsl::elm::ListItem("六级: " + std::to_string((this->_fanCurveTable + 5)->temperature_c) + "℃ | " + std::to_string((int)((this->_fanCurveTable + 5)->fanLevel_f * 100)) + "%");
    this->_p6Label = new tsl::elm::ListItem("七级: " + std::to_string((this->_fanCurveTable + 6)->temperature_c) + "℃ | " + std::to_string((int)((this->_fanCurveTable + 6)->fanLevel_f * 100)) + "%");
    this->_p7Label = new tsl::elm::ListItem("八级: " + std::to_string((this->_fanCurveTable + 7)->temperature_c) + "℃ | " + std::to_string((int)((this->_fanCurveTable + 7)->fanLevel_f * 100)) + "%");
    this->_p8Label = new tsl::elm::ListItem("九级: " + std::to_string((this->_fanCurveTable + 8)->temperature_c) + "℃ | " + std::to_string((int)((this->_fanCurveTable + 8)->fanLevel_f * 100)) + "%");
    this->_p9Label = new tsl::elm::ListItem("十级: " + std::to_string((this->_fanCurveTable + 9)->temperature_c) + "℃ | " + std::to_string((int)((this->_fanCurveTable + 9)->fanLevel_f * 100)) + "%");

    if (IsRunning() != 0)
    {
        this->_enabledBtn = new tsl::elm::ToggleListItem("调整风扇转速", true);
    }
    else
    {
        this->_enabledBtn = new tsl::elm::ToggleListItem("调整风扇转速", false);
    }
}

MainMenu::~MainMenu()
{
    CloseSensors();
}

tsl::elm::Element* MainMenu::createUI()
{
    this->_tableIsChanged = false;

    auto frame = new tsl::elm::OverlayFrame("风扇控制", "v1.0.5+");

    auto list = new tsl::elm::List();

    this->_enabledBtn->setStateChangedListener([this](bool state)
    {
	    if (state)
        {
            CreateB2F();
            const NcmProgramLocation programLocation{
                .program_id = SysFanControlID,
                .storageID = NcmStorageId_None,
            };
            u64 pid = 0;
            pmshellLaunchProgram(0, &programLocation, &pid);
            return true;
		}
        else
        {
            RemoveB2F();
            pmshellTerminateProgram(SysFanControlID);
            return true;
        }
	    return false;
    });
    list->addItem(this->_enabledBtn);

    // Add real-time monitoring section
    list->addItem(new tsl::elm::CategoryHeader("当前系统状态", true));
    list->addItem(this->_socTempLabel);
    list->addItem(this->_fanSpeedLabel);

    list->addItem(new tsl::elm::CategoryHeader("调整风扇转速曲线", true));
    this->_p0Label->setClickListener([this](uint64_t keys)
    {
	    if (keys & HidNpadButton_A)
        {
			tsl::changeTo<SelectMenu>(0, this->_fanCurveTable, &this->_tableIsChanged);
			return true;
		}
		return false;
    });
    list->addItem(this->_p0Label);

    this->_p1Label->setClickListener([this](uint64_t keys)
    {
	    if (keys & HidNpadButton_A)
        {
			tsl::changeTo<SelectMenu>(1, this->_fanCurveTable, &this->_tableIsChanged);
			return true;
		}
		return false;
    });
    list->addItem(this->_p1Label);

    this->_p2Label->setClickListener([this](uint64_t keys)
    {
	    if (keys & HidNpadButton_A)
        {
			tsl::changeTo<SelectMenu>(2, this->_fanCurveTable, &this->_tableIsChanged);
			return true;
		}
		return false;
    });
    list->addItem(this->_p2Label);

    this->_p3Label->setClickListener([this](uint64_t keys)
    {
	    if (keys & HidNpadButton_A)
        {
		    tsl::changeTo<SelectMenu>(3, this->_fanCurveTable, &this->_tableIsChanged);
			return true;
		}
		return false;
    });
    list->addItem(this->_p3Label);

    this->_p4Label->setClickListener([this](uint64_t keys)
    {
	    if (keys & HidNpadButton_A)
        {
				tsl::changeTo<SelectMenu>(4, this->_fanCurveTable, &this->_tableIsChanged);
				return true;
	    }
	    return false;
    });
    list->addItem(this->_p4Label);
        this->_p5Label->setClickListener([this](uint64_t keys)
    {
        if (keys & HidNpadButton_A)
        {
            tsl::changeTo<SelectMenu>(5, this->_fanCurveTable, &this->_tableIsChanged);
            return true;
        }
        return false;
    });
    list->addItem(this->_p5Label);

    this->_p6Label->setClickListener([this](uint64_t keys)
    {
        if (keys & HidNpadButton_A)
        {
            tsl::changeTo<SelectMenu>(6, this->_fanCurveTable, &this->_tableIsChanged);
            return true;
        }
        return false;
    });
    list->addItem(this->_p6Label);

    this->_p7Label->setClickListener([this](uint64_t keys)
    {
        if (keys & HidNpadButton_A)
        {
            tsl::changeTo<SelectMenu>(7, this->_fanCurveTable, &this->_tableIsChanged);
            return true;
        }
        return false;
    });
    list->addItem(this->_p7Label);

    this->_p8Label->setClickListener([this](uint64_t keys)
    {
        if (keys & HidNpadButton_A)
        {
            tsl::changeTo<SelectMenu>(8, this->_fanCurveTable, &this->_tableIsChanged);
            return true;
        }
        return false;
    });
    list->addItem(this->_p8Label);

    this->_p9Label->setClickListener([this](uint64_t keys)
    {
        if (keys & HidNpadButton_A)
        {
            tsl::changeTo<SelectMenu>(9, this->_fanCurveTable, &this->_tableIsChanged);
            return true;
        }
        return false;
    });
    list->addItem(this->_p9Label);

    frame->setContent(list);

    return frame;
}

void MainMenu::update()
{
    static u64 counter = 0;
    counter++;

    // Update readings every 6-12 frames (0.1-0.2s at 60 FPS)
    if (counter % 6 == 0) {
        // Get SOC temperature
        float socTemp = GetSOCTemperature();
        if (socTemp >= 0) {
            this->_socTempLabel->setText("核心温度: " + std::to_string((int)socTemp) + "℃");
        } else {
            this->_socTempLabel->setText("核心温度: 错误");
        }

        // Get fan speed
        float fanSpeed = GetFanSpeed();
        if (fanSpeed >= 0) {
            this->_fanSpeedLabel->setText("风扇转速: " + std::to_string((int)fanSpeed) + "%");
        } else {
            this->_fanSpeedLabel->setText("风扇转速: 错误");
        }
    }

    if(this->_tableIsChanged)
    {
        this->_p0Label->setText("一级: " + std::to_string(this->_fanCurveTable->temperature_c) + "℃ | " + std::to_string((int)(this->_fanCurveTable->fanLevel_f * 100)) + "%");
        this->_p1Label->setText("二级: " + std::to_string((this->_fanCurveTable + 1)->temperature_c) + "℃ | " + std::to_string((int)((this->_fanCurveTable + 1)->fanLevel_f * 100)) + "%");
        this->_p2Label->setText("三级: " + std::to_string((this->_fanCurveTable + 2)->temperature_c) + "℃ | " + std::to_string((int)((this->_fanCurveTable + 2)->fanLevel_f * 100)) + "%");
        this->_p3Label->setText("四级: " + std::to_string((this->_fanCurveTable + 3)->temperature_c) + "℃ | " + std::to_string((int)((this->_fanCurveTable + 3)->fanLevel_f * 100)) + "%");
        this->_p4Label->setText("五级: " + std::to_string((this->_fanCurveTable + 4)->temperature_c) + "℃ | " + std::to_string((int)((this->_fanCurveTable + 4)->fanLevel_f * 100)) + "%");
        this->_p5Label->setText("六级: " + std::to_string((this->_fanCurveTable + 5)->temperature_c) + "℃ | " + std::to_string((int)((this->_fanCurveTable + 5)->fanLevel_f * 100)) + "%");
        this->_p6Label->setText("七级: " + std::to_string((this->_fanCurveTable + 6)->temperature_c) + "℃ | " + std::to_string((int)((this->_fanCurveTable + 6)->fanLevel_f * 100)) + "%");
        this->_p7Label->setText("八级: " + std::to_string((this->_fanCurveTable + 7)->temperature_c) + "℃ | " + std::to_string((int)((this->_fanCurveTable + 7)->fanLevel_f * 100)) + "%");
        this->_p8Label->setText("九级: " + std::to_string((this->_fanCurveTable + 8)->temperature_c) + "℃ | " + std::to_string((int)((this->_fanCurveTable + 8)->fanLevel_f * 100)) + "%");
        this->_p9Label->setText("十级: " + std::to_string((this->_fanCurveTable + 9)->temperature_c) + "℃ | " + std::to_string((int)((this->_fanCurveTable + 9)->fanLevel_f * 100)) + "%");

        this->_tableIsChanged = false;
    }
}