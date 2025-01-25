#include "module.h"

class CommandNSSetProperty : public Command
{
public:
    CommandNSSetProperty(Module *creator) : Command(creator, "nickserv/set/property", 2, 2)
    {
        this->SetDesc("Set a custom property on your account");
        this->SetSyntax("\037property\037 \037value\037");
    }

    void Execute(CommandSource &source, const std::vector<Anope::string> &params) override
    {
        const Anope::string &property = params[0];
        const Anope::string &value = params[1];
        NickAlias *na = NickAlias::Find(source.GetAccount()->GetNick());

        if (!na || !na->nc)
        {
            source.Reply("You must be identified to use this command.");
            return;
        }

        // Store the property as metadata
        na->nc->ExtendMetadata("property:" + property, value);

        source.Reply("Property \002%s\002 set to \002%s\002.", property.c_str(), value.c_str());
    }

    bool OnHelp(CommandSource &source, const Anope::string &) override
    {
        source.Reply("Syntax: \002SET PROPERTY <property> <value>\002\n"
                     "Allows you to set a custom property for your nickname account.\n"
                     "These properties can be used to store additional metadata.");
        return true;
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
