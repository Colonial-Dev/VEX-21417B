#pragma once
//It's time to get funky

bool displayUpdateFlag = true;

class MenuItem
{
  public:
    std::function<void()> operation;
    std::string name;

    MenuItem(const std::function<void()>& action, std::string ident)
    {
      operation = action;
      name = ident;
    }

    std::string GetName() { return name; }

    void DoOperation() { operation(); }
};

class MenuLevel
{
  public:
    std::vector<MenuItem> items;
    std::string name;

  MenuLevel(std::vector<MenuItem> itemvec, std::string ident)
  {
    items = itemvec;
    name = ident;
  }

  std::string GetName() { return name; }
};

class MenuManager
{
  public:
    std::vector<MenuLevel> levels;
    int level_index = 0;
    int item_index = 0;

  void Update(std::vector<MenuLevel> levelvec)
  {
    levels = levelvec;
  }

  void Page(bool direction)
  {
    MenuLevel level = levels.at(level_index);
    std::vector<MenuItem> items = level.items;

    if(direction) { item_index++; } //Page right
    else { item_index--; }
    if(item_index > items.size() - 1) { item_index = 0; }
    else if(item_index < 0) { item_index = items.size() - 1; }
    displayUpdateFlag = true;
  }

  std::string GetCurrentItemName()
  {
    MenuLevel level = levels.at(level_index);
    std::vector<MenuItem> items = level.items;
    return items.at(item_index).name;
  }
  
  bool InHotkeyMode()
  {
    MenuLevel level = levels.at(level_index);
    return level.name == "Hotkey";
  }

  std::string GetCurrentLevelName()
  {
    MenuLevel level = levels.at(level_index);
    return level.name;
  }

  void GotoLevel(std::string name) //"efficiency? what's that?"
  {
    for(int i = 0; i < levels.size(); i++)
    {
      MenuLevel lv = levels.at(i);
      if(lv.GetName() == name) { level_index = i; item_index = 0; displayUpdateFlag = true; return; }
    }
  }

  void DoOperation()
  {
    MenuLevel level = levels.at(level_index);
    MenuItem item = level.items.at(item_index);
    item.DoOperation();
    displayUpdateFlag = true;
  }
};
