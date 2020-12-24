using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace cppcx.Core.Models
{
    public class CxMemberFunction
    {
        public CxFunction Function { get; set; }
        public CxVisibility Visibility { get; set; } = CxVisibility.Public;
        public bool IsVirtual { get; set; } = false;
        public bool IsOverriden { get; set; } = false;

    }
}
