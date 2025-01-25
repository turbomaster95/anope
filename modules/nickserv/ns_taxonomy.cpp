#include "module.h"

class CommandNSTaxonomy : public Command
{
public:
    CommandNSTaxonomy(Module *creator) : Command(creator, "nickserv/taxonomy", 0, 1)
    {
        this->SetDesc("View all properties set on your account or another user's account");
        this->SetSyntax("[\037nickname\037]");
    }

    void Execute(CommandSource &source, const std::vector<Anope::string> &params) override
    {
        NickAlias *na = nullptr;

        if (params.empty()) // No nickname provided, use the command source's account
        {
            na = NickAlias::Find(source.GetAccount()->GetNick());
        }
        else // A nickname is provided
        {
            const Anope::string &nickname = params[0];
            na = NickAlias::Find(nickname);

            if (!na)
            {
                source.Reply("Nickname \002%s\002 is not registered.", nickname.c_str());
                return;
            }

            if (!source.GetAccount() || !source.GetAccount()->IsServicesOper())
            {
                source.Reply("You do not have permission to view properties for other users.");
                return;
            }
        }

        if (!na || !na->nc)
        {
            source.Reply("You must be identified to use this command.");
            return;
        }

        bool has_properties = false;

        // Iterate through metadata to find properties
        for (const auto &metadata : na->nc->GetExt())
        {
            if (metadata.first.find("property:") == 0) // Check if key starts with "property:"
            {
                has_properties = true;
                source.Reply("\002%s\002: \002%s\002",
                             metadata.first.substr(9).c_str(), // Remove "property:" prefix
                             metadata.second->data.c_str());
            }
        }

        if (!has_properties)
        {
            if (params.empty())
                source.Reply("No properties are set on your account.");
            else
                source.Reply("No properties are set on \002%s\002's account.", na->nick.c_str());
        }
    }

    bool OnHelp(CommandSource &source, const Anope::string &) override
    {
        source.Reply("Syntax: \002TAXONOMY [nickname]\002\n"
                     "Displays all properties set on your nickname account, or on the account of the specified nickname.\n"
                     "You must have the appropriate permissions to view another user's properties.");
        return true;
    }
};

class NSTaxonomy : public Module
{
    CommandNSTaxonomy commandnstaxonomy;

public:
    NSTaxonomy(const Anope::string &modname, const Anope::string &creator)
        : Module(modname, creator, THIRD), commandnstaxonomy(this)
    {
        this->SetAuthor("Coder");
        this->SetVersion("1.1");
    }
};

MODULE_INIT(NSTaxonomy)
