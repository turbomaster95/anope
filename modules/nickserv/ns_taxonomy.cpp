#include "module.h"
#include "account.h"

class CommandNSTaxonomy : public Command
{
public:
    CommandNSTaxonomy(Module *creator) : Command(creator, "nickserv/taxonomy", 0, 1)
    {
        this->SetDesc("List all properties on a specified account");
        this->SetSyntax("[nickname]");
    }

    void Execute(CommandSource &source, const std::vector<Anope::string> &params) override
    {
        NickAlias *na = nullptr;
        if (params.empty())
        {
            na = NickAlias::Find(source.GetAccount()->display);
            if (!na)
            {
                source.Reply("You are not logged in to an account.");
                return;
            }
        }
        else
        {
            const Anope::string &nickname = params[0];
            na = NickAlias::Find(nickname);
            if (!na)
            {
                source.Reply("The nickname '%s' was not found.", nickname.c_str());
                return;
            }
        }

        source.Reply("Properties for account %s:", na->nc->display.c_str());

        // Accessing extensions directly using GetItems to get all metadata
        const Anope::string metadataKey = "property:";
        for (const auto &item : na->nc->GetItems())
        {
            // Attempt to cast item to Anope::string or another suitable type
            Anope::string *strItem = dynamic_cast<Anope::string *>(item);
            if (strItem)
            {
                if (strItem->find(metadataKey) == 0)
                {
                    Anope::string value = *strItem; // Assuming item is of type Anope::string
                    source.Reply("  %s", value.c_str());
                }
            }
            else
            {
                // Handle case where item isn't an Anope::string or log an error
                source.Reply("  Non-string item encountered.");
            }
        }
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
        this->SetVersion("1.0");
    }
};

MODULE_INIT(NSTaxonomy)
