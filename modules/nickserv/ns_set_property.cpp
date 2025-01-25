#include "module.h"
#include "account.h"

class CommandNSSetProperty : public Command
{
public:
    CommandNSSetProperty(Module *creator) : Command(creator, "nickserv/set_property", 2, 2)
    {
        this->SetDesc("Set a custom property on your account");
        this->SetSyntax("<property> <value>");
    }

    void Execute(CommandSource &source, const std::vector<Anope::string> &params) override
    {
        const Anope::string &property = params[0];
        const Anope::string &value = params[1];

        NickAlias *na = NickAlias::Find(source.GetAccount()->display);
        if (!na)
        {
            source.Reply("You are not logged in to an account.");
            return;
        }

        na->nc->Extend<Anope::string>("property:" + property, value);
        source.Reply("Property '%s' set to '%s'.", property.c_str(), value.c_str());
    }
};

class NSSetProperty : public Module
{
    CommandNSSetProperty commandnssetproperty;

public:
    NSSetProperty(const Anope::string &modname, const Anope::string &creator)
        : Module(modname, creator, THIRD), commandnssetproperty(this)
    {
        this->SetAuthor("Coder");
        this->SetVersion("1.0");
    }
};

MODULE_INIT(NSSetProperty)
