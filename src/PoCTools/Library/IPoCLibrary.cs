using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PoCTools.Library
{
    public interface IPoCLibrary
    {
        public Task LoadLibrary(string path);
        public Task SaveLibrary(string path);
        public List<Card>? Cards { get; }
    }
}
