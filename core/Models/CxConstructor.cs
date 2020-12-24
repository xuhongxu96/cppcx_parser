using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace cppcx.Core.Models
{
    public class CxConstructor
    {
        public List<CxParameter> Parameters { get; init; } = new List<CxParameter>();
        public List<CxInitializationListItem> InitializationList { get; init; } = new List<CxInitializationListItem>();
        public CxVisibility Visibility { get; set; } = CxVisibility.Public;
        public CxCode Code { get; set; }
    }
}
