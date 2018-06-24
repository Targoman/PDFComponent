################################################################################
#   Copyright 2018 by Targoman.com
#
#   Tarjomyar is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU Lesser General Public License for more details.
#
#   You should have received a copy of the GNU Lesser General Public License
#   along with Targoman. If not, see <http://www.gnu.org/licenses/>.
################################################################################

TEMPLATE = subdirs

include(../Project.pri)

SUBDIRS += third_party PDFLA
CONFIG += ordered
