#ifndef MENU_H
#define MENU_H
//It's time to get funky

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
    int level_index;
    int item_index;

  MenuManager(std::vector<MenuLevel> levelvec)
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
  }

  std::string GetCurrentItemName()
  {
    MenuLevel level = levels.at(level_index);
    std::vector<MenuItem> items = level.items;
    return items.at(item_index).name;
  }

  void GotoLevel(std::string name) //"efficiency? what's that?"
  {
    for(int i = 0; i < levels.size(); i++)
    {
      MenuLevel lv = levels.at(i);
      if(lv.GetName() == name) { level_index = i; item_index = 0; return; }
    }
  }

  void GotoItem(std::string name)
  {
    MenuLevel level = levels.at(level_index);
    std::vector<MenuItem> items = level.items;

    for(int i = 0; i < items.size(); i++)
    {
      MenuItem it = items.at(i);
      if(it.GetName() == name) { item_index = i; return; }
    }
  }

  void DoOperation()
  {
    MenuLevel level = levels.at(level_index);
    MenuItem item = level.items.at(item_index);
    item.DoOperation();
  }
};

#endif
